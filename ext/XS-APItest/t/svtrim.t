#!perl -w

use Test::More;

use XS::APItest qw<sv_ltrim sv_rtrim>;

sub ltrim_ok {
    my ($input, $expected, $desc) = @_;
    sv_ltrim(my $got = $input);
    is($got, $expected, "ltrim: $desc");
}

sub rtrim_ok {
    my ($input, $expected, $desc) = @_;
    sv_rtrim(my $got = $input);
    is($got, $expected, "rtrim: $desc");
}

ltrim_ok('', '', 'empty string');
ltrim_ok("\x09\x0A\x0B\x0C\x0D\x20\x85\xA0", '', 'empty except for whitespace');
ltrim_ok('foo', 'foo', 'no whitespace');
ltrim_ok(' foo', 'foo', 'one leading space');

ltrim_ok("\x09\x0A\x0B\x0C\x0D\x20\x85\xA0foo", 'foo', 'all single-byte space');

ltrim_ok("fo\N{LATIN SMALL LIGATURE OE}", "fo\N{LATIN SMALL LIGATURE OE}",
         'no whitespace, utf-8');

ltrim_ok(" fo\N{LATIN SMALL LIGATURE OE}", "fo\N{LATIN SMALL LIGATURE OE}",
         'one leading space, utf-8');

ltrim_ok("\x{2000} foo", "foo", "one leading unicode space");

rtrim_ok('', '', 'empty string');
rtrim_ok("\x09\x0A\x0B\x0C\x0D\x20\x85\xA0", '', 'empty except for whitespace');
rtrim_ok('foo', 'foo', 'no whitespace');
rtrim_ok('foo ', 'foo', 'one trailing space');

rtrim_ok("foo\x09\x0A\x0B\x0C\x0D\x20\x85\xA0", 'foo', 'all single-byte space');

rtrim_ok("fo\N{LATIN SMALL LIGATURE OE}", "fo\N{LATIN SMALL LIGATURE OE}",
         'no whitespace, utf-8');

rtrim_ok("fo\N{LATIN SMALL LIGATURE OE} ", "fo\N{LATIN SMALL LIGATURE OE}",
         'one trailing space, utf-8');

rtrim_ok("foo\x{2000}", "foo", "one trailing unicode space");

done_testing();
