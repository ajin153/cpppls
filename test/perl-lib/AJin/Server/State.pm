package AJin::Server::State;

our $CONFIG   = {perltidy => {}, perlcritic => {}};
our $CLIENT_CAPABILITIES = {
    "textDocument" => {
        "publishDiagnostics" => {
            "versionSupport" => "true"
        }
    }
};
