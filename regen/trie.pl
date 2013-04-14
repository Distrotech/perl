#! /usr/bin/perl

# Regenerate (overwriting only if changed):
#
#   trie.h
#
# from Unicode data as reported by /\s/u
#
# Accepts the standard regen_lib -q and -v args
#
# This program need be run only when the definition of /\s/u changes, or
# when the trie implementation changes in relevant ways.

use v5.12;
use warnings;

require 'regen/regen_lib.pl';

use Carp qw<croak>;

{
    my @ws = find_whitespace();

    my $trie_h = open_new('trie.h', '>', {
        by        => 'regen/trie.pl',
        from      => 'Unicode property definitions',
        file      => 'trie.h',
        style     => '*',
        copyright => [2013],
    });

    generate($trie_h, (
        symbol  => 'forward_whitespace',
        prefix  => 'WS_',
        strings => [map bytes($_), @ws],
    ));

    generate($trie_h, (
        symbol  => 'reverse_whitespace',
        prefix  => 'REVWS_',
        strings => [map scalar(reverse bytes($_)), @ws],
    ));

    read_only_bottom_close_and_rename($trie_h);
}

sub find_whitespace {
    my @codepoints = grep chr =~ /\s/u, 0 .. 0x10ffff
        or croak('No whitespace codepoints found');
    return @codepoints;
}

sub bytes {
    my ($codepoint) = @_;
    my $s = chr $codepoint;
    utf8::encode($s);
    return $s;
}

sub generate {
    my ($fh, @generator_args) = @_;
    my $generator = Generator->new(@generator_args);
    $generator->emit($fh);
}

BEGIN {
    package Generator;

    sub new {
        my $class = shift;
        bless {
            prefix    => '',
            @_,
            root      => {},
            label_pos => {},
        }, $class;
    }

    sub emit {
        my ($self, $fh) = @_;
        $fh->print("$_\n") for (
            $self->_label_definitions, '',
            "static const Edge $self->{symbol}\[] = {",
            (map "    $_", $self->_edge_definitions),
            "};", '',
        );
    }

    sub _build_root {
        my $self = shift;

        for my $s (@{ $self->{strings} }) {
            my @bytes = map ord, split //, $s;
            my $node = $self->{root};
            for (;;) {
                push @{ $node->{heads} }, join '',
                    map sprintf('%.2X', $_), @bytes;
                my $tail = shift @bytes;
                my ($edge) = grep $_->[0] == $tail, @{ $node->{edges} };
                if (!$edge) {
                    $edge = [$tail];
                    push @{ $node->{edges} }, $edge;
                }
                last if !@bytes;
                $node = \%{ $edge->[1] };
            }
        }
    }

    sub _build_edges {
        my $self = shift;

        return if $self->{edges};

        my $root_label = $self->_child_label('');

        $self->_build_root;
        my @edges;
        my $pos = $self->{label_pos};
        my @agenda = [$root_label => $self->{root}];
        while (@agenda) {
            my ($label, $node) = @{ shift @agenda };
            $pos->{$label} = @edges;

            my (@more, @new);
            for my $edge (@{ $node->{edges} }) {
                my $child = $edge->[1];
                my $child_label = $self->_child_label($child);
                if ($child_label ne $root_label && !exists $pos->{$child_label}) {
                    $pos->{$child_label} = -1;
                    push @more, [$child_label => $child];
                }
                push @new, [$edge->[0], $child_label, 0];
            }
            push @edges, sort { $a->[0] <=> $b->[0] } @new;
            $edges[-1][2] = 1;  # mark last edge as node-ending
            unshift @agenda, @more;
        }

        $self->{edges} = [map sprintf('EDGE(0x%.2X, %s, %u),', @$_), @edges];
    }

    sub _label_definitions {
        my $self = shift;

        $self->_build_edges;

        my @labels = $self->_ordered_labels;
        my $edges = $self->{edges};
        my $width = $self->_width(@labels);
        my $pos = $self->{label_pos};

        my $end_label = "$self->{prefix}LEN";

        local $pos->{$end_label} = @$edges;
        return map sprintf('#define %-*s %*u',
                           $width, $_, length $#$edges, $pos->{$_}),
                   @labels, $end_label;
    }

    sub _edge_definitions {
        my $self = shift;

        $self->_build_edges;

        my @edges    = @{ $self->{edges} };
        my @labels   = $self->_ordered_labels;
        my %label_at = reverse %{ $self->{label_pos} };
        my $width    = $self->_width(@edges);

        my @ret;
        while (my ($i, $edge) = each @edges) {
            if (my $label = $label_at{$i}) {
                push @ret, sprintf '%-*s /* %*u: %s */',
                    $width, $edge, length $#edges, $i, $label;
            }
            else {
                push @ret, $edge;
            }
        }

        return @ret;
    }

    sub _child_label {
        my ($self, $child) = @_;
        return "$self->{prefix}ROOT" if !$child;
        my @ranges;
        for (sort @{ $child->{heads} }) {
            my ($prefix, $tail) = /\A(.*)(..)\z/ms;
            my $prev = sprintf '%s%02X', $prefix, hex($tail) - 1;
            if (@ranges && $ranges[-1][1] eq $prev) {
                $ranges[-1][1] = $_;
            }
            else {
                push @ranges, [$_, $_];
            }
        }
        return "$self->{prefix}MATCH_" . join '_',
            map +($_->[0] . ($_->[0] eq $_->[1] ? '' : "to$_->[1]")), @ranges;
    }

    sub _ordered_labels {
        my $self = shift;
        my $label_pos = $self->{label_pos};
        return sort { $label_pos->{$a} <=> $label_pos->{$b} } keys %$label_pos;
    }

    sub _width {
        my $self = shift;
        my $max = 0;
        $max < $_ and $max = $_ for map length, @_;
        return $max;
    }
}
