use strict;
use warnings;

use Test2::Global;

my ($LOADED, $INIT);
BEGIN {
    $INIT   = Test2::Global::test2_init_done;
    $LOADED = Test2::Global::test2_load_done;
};

use Test2::IPC;
BEGIN { require "t/tools.pl" };
use Test2::Util qw/get_tid/;
my $CLASS = 'Test2::Global';

# Ensure we do not break backcompat later by removing anything
ok(Test2::Global->can($_), "$_ method is present") for qw{
    test2_init_done
    test2_load_done

    test2_pid
    test2_tid
    test2_stack
    test2_no_wait

    test2_add_callback_context_init
    test2_add_callback_context_release
    test2_add_callback_exit
    test2_add_callback_post_load

    test2_ipc
    test2_ipc_drivers
    test2_ipc_add_driver
    test2_ipc_polling
    test2_ipc_disable_polling
    test2_ipc_enable_polling

    test2_formatter
    test2_formatters
    test2_formatter_add
    test2_formatter_set
};

ok(!$LOADED, "Was not load_done right away");
ok(!$INIT, "Init was not done right away");
ok(Test2::Global::test2_load_done, "We loaded it");

# Note: This is a check that stuff happens in an END block.
{
    {
        package FOLLOW;

        sub DESTROY {
            return if $_[0]->{fixed};
            print "not ok - Did not run end ($_[0]->{name})!";
            $? = 255;
            exit 255;
        }
    }

    our $kill1 = bless {fixed => 0, name => "Custom Hook"}, 'FOLLOW';
    Test2::Global::test2_add_callback_exit(
        sub {
            print "# Running END hook\n";
            $kill1->{fixed} = 1;
        }
    );

    our $kill2 = bless {fixed => 0, name => "set exit"}, 'FOLLOW';
    my $old = Test2::Global::Instance->can('set_exit');
    no warnings 'redefine';
    *Test2::Global::Instance::set_exit = sub {
        $kill2->{fixed} = 1;
        print "# Running set_exit\n";
        $old->(@_);
    };
}

ok($CLASS->can('test2_init_done')->(), "init is done.");
ok($CLASS->can('test2_load_done')->(), "Test2 is finished loading");

is($CLASS->can('test2_pid')->(), $$, "got pid");
is($CLASS->can('test2_tid')->(), get_tid(), "got tid");

ok($CLASS->can('test2_stack')->(), 'got stack');
is($CLASS->can('test2_stack')->(), $CLASS->can('test2_stack')->(), "always get the same stack");

ok($CLASS->can('test2_ipc')->(), 'got ipc');
is($CLASS->can('test2_ipc')->(), $CLASS->can('test2_ipc')->(), "always get the same IPC");

is_deeply([$CLASS->can('test2_ipc_drivers')->()], [qw/Test2::IPC::Driver::Files/], "Got driver list");

# Verify it reports to the correct file/line, there was some trouble with this...
my $file = __FILE__;
my $line = __LINE__ + 1;
my $warnings = warnings { $CLASS->can('test2_ipc_add_driver')->('fake') };
like(
    $warnings->[0],
    qr{^IPC driver fake loaded too late to be used as the global ipc driver at \Q$file\E line $line},
    "got warning about adding driver too late"
);

is_deeply([$CLASS->can('test2_ipc_drivers')->()], [qw/fake Test2::IPC::Driver::Files/], "Got updated list");

ok($CLASS->can('test2_ipc_polling')->(), "Polling is on");
$CLASS->can('test2_ipc_disable_polling')->();
ok(!$CLASS->can('test2_ipc_polling')->(), "Polling is off");
$CLASS->can('test2_ipc_enable_polling')->();
ok($CLASS->can('test2_ipc_polling')->(), "Polling is on");

ok($CLASS->can('test2_formatter')->(), "Got a formatter");
is($CLASS->can('test2_formatter')->(), $CLASS->can('test2_formatter')->(), "always get the same Formatter (class name)");

my $ran = 0;
$CLASS->can('test2_add_callback_post_load')->(sub { $ran++ });
is($ran, 1, "ran the post-load");

like(
    exception { $CLASS->can('test2_formatter_set')->() },
    qr/No formatter specified/,
    "formatter_set requires an argument"
);

like(
    exception { $CLASS->can('test2_formatter_set')->('fake') },
    qr/Global Formatter already set/,
    "formatter_set doesn't work after initialization",
);

ok(!$CLASS->can('test2_no_wait')->(), "no_wait is not set");
$CLASS->can('test2_no_wait')->(1);
ok($CLASS->can('test2_no_wait')->(), "no_wait is set");
$CLASS->can('test2_no_wait')->(undef);
ok(!$CLASS->can('test2_no_wait')->(), "no_wait is not set");

done_testing;
