// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


define(
    ['jquery', 'dockspawn'],

    function($, _, ds) {
        console.log("Dockspawn loaded...");

        // Turn on tooltips, ugh these are annoying 
        // (don't forget to include jquery_ui if using this again...)
        // $( document ).tooltip();

        // Convert a div to the dock manager.  Panels can then be docked on to it
        var divDockManager = $("#my_dock_manager")[0],
            dockManager = new dockspawn.DockManager(divDockManager);

        dockManager.initialize();

        // Let the dock manager element fill in the entire screen
        var onResized = function(e) {
            dockManager.resize(
                window.innerWidth - (divDockManager.clientLeft + divDockManager.offsetLeft), 
                window.innerHeight - (divDockManager.clientTop + divDockManager.offsetTop));
        }
        window.onresize = onResized;
        onResized(null);

        var status = new dockspawn.PanelContainer($("#status_window")[0], dockManager),

            editorDiv = $("#editor1_window")[0],
            editor1 = new dockspawn.PanelContainer(editorDiv, dockManager),
            noiseDiv = $("#noise_window"),
            noise = new dockspawn.PanelContainer(noiseDiv[0], dockManager),
            render = new dockspawn.PanelContainer($("#render_window")[0], dockManager),

            documentNode = dockManager.context.model.documentManagerNode,
            statusNode = dockManager.dockDown(documentNode, status, 0.1),
            renderNode = dockManager.dockFill(documentNode, render),
            noiseNode = dockManager.dockLeft(renderNode, noise, 0.25),
            editor1Node = dockManager.dockUp(noiseNode, editor1, 0.5);

        return { 
            manager: dockManager,
            noiseNode: noiseNode
        };
    }
);
