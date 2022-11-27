package AJin::Server::Request;

use parent 'AJin::Server::Message';

sub new {
    my ($class, $request) = @_;

    return bless $request, $class;
}

sub service {
    my ($self, $server) = @_;
    return;
}

sub handle_response {
    my ($self, $response, $server) = @_;

    return;
}

1;
