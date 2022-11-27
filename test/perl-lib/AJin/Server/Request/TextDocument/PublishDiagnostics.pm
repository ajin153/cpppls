package AJin::Server::Request::TextDocument::PublishDiagnostics;

use URI;
use File::Basename;

use AJin::Parser::Document;

sub new
{
    my ($class, %args) = @_;

    return if (ref $AJin::Server::State::CONFIG ne 'HASH');

    my $uri = URI->new($args{uri});
    return if (ref $uri ne 'URI::file');

    my $self = bless {
                      method => 'textDocument/publishDiagnostics',
                      params => {
                                 uri         => $uri->as_string,
                                 diagnostics => []
                                },
                      notification => 1
                     },
      $class;

    my (undef, $dir) = File::Basename::fileparse($uri->file);

    my $source = $uri->file;
    my $text   = AJin::Parser::Document->text_from_uri($uri->as_string);
    $source = $text if (ref $text eq 'SCALAR');
    my $version                    = AJin::Parser::Document::uri_version($uri->as_string);
    my $client_has_version_support = $AJin::Server::State::CLIENT_CAPABILITIES->{textDocument}{publishDiagnostics}{versionSupport};
    $self->{params}{version} = $version if (length $version and $client_has_version_support);

    # If closing, return empty list of diagnostics.
    return Future->done($self) if $args{close};

    my @futures;

    push @futures, get_compilation_errors($source, $dir) if (defined $PLS::Server::State::CONFIG->{syntax}{enabled} and $PLS::Server::State::CONFIG->{syntax}{enabled});
    push @futures, get_perlcritic_errors($source, $uri->file)
      if (defined $PLS::Server::State::CONFIG->{perlcritic}{enabled} and $PLS::Server::State::CONFIG->{perlcritic}{enabled});

    return Future->wait_all(@futures)->then(
        sub {
            my $current_version = PLS::Parser::Document::uri_version($uri->as_string);

            # No version will be returned if the document has been closed.
            # Since the only way we got here is if the document is open, we
            # should return nothing, since any diagnostics we return will be from
            # when the document was still open.
            return Future->done(undef) unless (length $current_version);

            # If the document has been updated since the diagnostics were created,
            # send nothing back. The next update will re-trigger the diagnostics.
            return Future->done(undef) if (length $version and $current_version > $version);

            @{$self->{params}{diagnostics}} = map { $_->result } @_;

            return Future->done($self);
        }
    );
} ## end sub new
