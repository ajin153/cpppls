#include "didopen.h"
#include "../../../../tools/tools.h"
#include "../../../../parser/uri.h"
#include "../../../server/notification/textdocument/publishdiagnostics.h"

/*
{
    "jsonrpc": "2.0",
    "method": "textDocument/didOpen",
    "params": {
        "textDocument": {
            "languageId": "perl",
            "text": "package AJin::PPI::Document;\r\n\r\nuse strict;\r\nuse warnings;\r\n\r\nour @ISA = \"AJin::PPI::Node\";\r\n\r\nuse AJin::PPI::Lexer;\r\n\r\n# Document类对象结构如下：\r\n# {\r\n#   children => [],\r\n#   readonly => ,\r\n#   tab_width => 1\r\n# }\r\n\r\nsub new\r\n{\r\n    local $_;\r\n    my $class = ref $_[0] ? ref shift : shift; # $_[0] => AJin::PPI::Document\r\n\r\n    unless ( @_ ) {\r\n        my $self = $class->SUPER::new;\r\n        $self->{readonly} = ! 1;\r\n        $self->{tab_width} = 1;\r\n        return $self;\r\n    }\r\n\r\n    my $source = shift;\r\n    my %attr = @_;\r\n\r\n    if (!defined $source) {\r\n        print \"An undefined value was passed to AJin::PPI::Document::new!\";\r\n    } elsif (!ref $source) {\r\n        $attr{filename} ||= $source;\r\n\r\n        my $document = AJin::PPI::Lexer->lex_file($source);\r\n        return $class->_setattr($document, %attr) if $document;\r\n    }\r\n}\r\n\r\nsub _setattr\r\n{\r\n    my ($class, $document, %attr) = @_;\r\n\r\n    $document->{readonly} = !! $attr{readonly};\r\n    $document->{filename} = $attr{filename};\r\n    return $document;\r\n}\r\n\r\n1;\r\n",
            "uri": "file:///d%3A/Users/User/Desktop/sf_project/z_tmp/cpppls/test/perl-lib/AJin/PPI/Document.pm",
            "version": 1
        }
    }
}
*/

void
DidOpenHandler::handle(const nlohmann::json& req_content)
{
    std::string raw_uri = req_content["params"]["textDocument"]["uri"];
    URI uri = URI(raw_uri);
    std::string perl_file_path = uri.get_path();

    if (!Tools::is_file_content_exist(perl_file_path)) {
        // 1.update file content
        Tools::update_file_content(perl_file_path);
    }

    if (!Tools::is_file_symbol_exist(perl_file_path)) {
        // 2.update file symbol and index
        Tools::update_file_symbol_and_index(perl_file_path);
    }

    // 3.send "textDocument/PublishDiagnostics" notification
    Handler *handler = new PublishDiagnosticsHandler();
    handler->handle(req_content);
}