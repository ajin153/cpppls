"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = void 0;
const node_1 = require("vscode-languageclient/node");
function activate(context) {
    console.log('Congratulations, your extension "cpppls" is now active!');
    const serverCmd = 'cpppls';
    const serverOptions = {
        run: {
            command: serverCmd,
            args: [],
        },
        debug: {
            command: serverCmd,
            args: [],
        },
    };
    const clientOptions = {
        documentSelector: [{ scheme: 'file', language: 'perl' }],
    };
    const disposable = new node_1.LanguageClient('cpppls', 'Perl Language Server Written by Cpp', serverOptions, clientOptions).start();
}
exports.activate = activate;
//# sourceMappingURL=extension.js.map