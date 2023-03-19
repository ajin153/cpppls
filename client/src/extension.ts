import * as vscode from 'vscode';

import {
    LanguageClient,
    LanguageClientOptions,
    ServerOptions,
} from 'vscode-languageclient/node'


export function activate(context: vscode.ExtensionContext) {
	console.log('Congratulations, your extension "cpppls" is now active!');

    const serverCmd = 'cpppls';
    const serverOptions: ServerOptions = {
        run: {
            command: serverCmd,
            args: [],
        },
        debug: {
            command: serverCmd,
            args: [],
        },
    };

    const clientOptions: LanguageClientOptions = {
        documentSelector: [{scheme: 'file', language: 'perl'}],
    };

    const disposable = new LanguageClient(
        'cpppls',
        'Perl Language Server Written by Cpp',
        serverOptions,
        clientOptions
    ).start();
}