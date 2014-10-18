// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module for the JSON Editor.

define(
    ['jquery', 'jsoneditor', 'gridster'],

    function($, JSONEditor) {
        console.log("editor loading...");

        var div = $("#editor1_window")[0],
            editor = new JSONEditor(div);
      
        editor.set({
          'msg': 'Drag and drop a glow scene JSON file here.'
        });

        $("#editor1_window").on("click", function(){
                var gridster = $(".gridster ul").gridster().data('gridster');
                gridster.disable().disable_resize();
                console.log("click editor / disable grid resize");
            });

        $(".gridster li header").on("click", function(){
                var gridster = $(".gridster ul").gridster().data('gridster');
                gridster.enable().enable_resize();
                console.log("click grid / enable grid resize");
            });

        return {
            div:    div,
            editor: editor
        };
    }
);

