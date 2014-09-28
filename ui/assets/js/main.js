// This file contains the main javascript for the interface.

require.config({
    baseUrl: 'js',
    paths: {
        // the left side is the module ID,
        // the right side is the path to
        // the jQuery file, relative to baseUrl.
        // Also, the path should NOT include
        // the '.js' file extension.
        jquery:     'jquery-1.11.1.min',
        jquery_ui:  'jquery-ui.min',
        dockspawn:  'dockspawn',
        jsoneditor: 'jsoneditor.min',
        cytoscape:  'cytoscape.min',
        jq_resize:  'jquery.resize',
        underscore: 'underscore'

        editor:     'glowEditor',
        noiseTree:  'glowNoiseTree',
        events:     'glowEvents'
    }
});

requirejs(
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
    }
);

requirejs(
    ['events'],
    function() {}
);


