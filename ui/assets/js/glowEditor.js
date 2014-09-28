// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module for the JSON Editor.

define(
    ['jquery', 'jsoneditor'],

    function($, JSONEditor) {
        console.log("Editor loaded...");

        var div = $("#editor1_window")[0],
            editor = new JSONEditor(div);

        editor.set({
          'msg': 'Drag and drop a glow scene JSON file here.'
        });

        return {
            div:    div,
            editor: editor
        };
    }
);

