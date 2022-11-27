package AJin::Server::Request::TextDocument::DidOpen;

use parent 'AJin::Server::Request';

use AJin::Parser::Document;

=head1 "didOpen"
{
    "jsonrpc": "2.0",
    "method": "textDocument/didOpen",
    "params": {
        "textDocument": {
            "languageId": "perl",
            "text": "",
            "uri": "file:///home/ajin/Desktop/ajpj/cpppls/test/test.pl",
            "version": 1
        }
    }
}
=cut

sub service {
    my ($self, $server) = @_;

    my $text_document = $self->{params}{textDocument};
    AJin::Parser::Document->open_file(%{$text_document});

    $server->


}
