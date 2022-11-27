package AJin::Parser::Document;

use strict;
use warnings;

use feature 'state'

my %FILES;
my %VERSIONS;

sub open_file
{
    # 这是个类方法
    my ($class, %args) = @_;

    return unless $args{languageId} eq 'perl';

    $FILES{$args{uri}}    = \($args{text});
    $VERSIONS{$args{uri}} = $args{version};

    return;
} ## end sub open_file

sub text_from_uri
{
    my (undef, $uri) = @_;

    if (ref $FILES{$uri} eq 'SCALAR')
    {
        return $FILES{$uri};
    }
    else
    {
        my $file = URI->new($uri);
        open my $fh, '<', $file->file or return;
        my $text = do { local $/; <$fh> };
        return \$text;
    } ## end else [ if (ref $FILES{$uri} eq...)]
} ## end sub text_from_uri

sub uri_version
{
    my ($uri) = @_;

    return $VERSIONS{$uri};
}
