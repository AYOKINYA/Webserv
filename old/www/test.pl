#!/usr/bin/env perl
=head1 DESCRIPTION
printenv — a CGI program that just prints its environment
=cut
print "Content-Type: text/plain\r\n\r\n";
for my $var ( sort keys %ENV ) {
    printf "%s=\"%s\"\n", $var, $ENV{$var};
}
