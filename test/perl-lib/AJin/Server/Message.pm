package AJin::Server::Message;

sub serialize
{
    my ($self) = @_;

    my %content = (
                   jsonrpc => '2.0',
                   %{$self}
                  );

    # 序列化json数据
    my $json = encode_json \%content;
    return \$json;
} ## end sub serialize

1;
