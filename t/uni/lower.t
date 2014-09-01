BEGIN {
    chdir 't' if -d 't';
    require "uni/case.pl";
    @INC = () unless is_miniperl();
    unshift @INC, qw(../lib .);
}

casetest(0, # No extra tests run here,
	"Lowercase_Mapping",
	 sub { lc $_[0] }, sub { my $a = ""; lc ($_[0] . $a) },
	 sub { lcfirst $_[0] }, sub { my $a = ""; lcfirst ($_[0] . $a) });
