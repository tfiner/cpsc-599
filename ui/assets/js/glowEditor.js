// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module for the JSON Editor.

define(
    ['jquery', 'jsoneditor', 'observer', 'gridster'],

    function($, JSONEditor, observer) {
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

        observer.subscribe("newNoiseNode", function(evt){
            // Find the right spot to append the new noise module to.
            var funcs       = editor.node.search('functions'),
                lastFunc    = funcs != undefined && funcs.length && 
                              _.has(funcs[0],'node') && _.has(funcs[0].node, 'childs') &&
                              funcs[0].node.childs.length > 0 ? 
                              funcs[0].node.childs.length - 1 : -1,
                              func = lastFunc >= 0 ? funcs[0].node.childs[lastFunc] : undefined;

            console.log("evt:", evt);

            if (func != undefined){
                func._onAppend(lastFunc, evt.node);
                editor.search("");
                func.scrollTo();
            }
        });

        observer.subscribe("newScene", function(evt) {
            editor.set(evt.scene);
        });

        observer.subscribe("treeSelectNode", function(evt) {
            // console.log("ids", ids);
            // console.log("editor", editor);
            var results = editor.search(evt.id),
                result = _.find(results, function(r) {
                    return r.node.getValue() == evt.id;
                });

            // console.log("results", results);
            // console.log("result", result);
            if (result) {
                editor.search("");
                editor.collapseAll();
                result.node.scrollTo();
            }
        });

        observer.subscribe("newView", function(evt) {
        });

        
        return {
            div:    div,
            editor: editor
        };
    }
);

