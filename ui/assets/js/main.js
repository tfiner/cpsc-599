// This file contains the main javascript for the interface.

require.config({
    baseUrl: 'js',
    paths: {
        // the left side is the module ID,
        // the right side is the path to
        // the jQuery file, relative to baseUrl.
        // Also, the path should NOT include
        // the '.js' file extension.
        jquery: 'jquery-1.11.1.min',
        dockspawn: 'dockspawn',
        jsoneditor: 'jsoneditor.min'
    }
});

requirejs(['jquery', 'dockspawn', 'jsoneditor'],
function($, ds, JSONEditor) {
    console.log("All loaded.");

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

    // Convert existing elements on the page into "Panels".
    // They can then be docked on to the dock manager
    // Panels get a titlebar and a close button, and can also be
    // converted to a floating dialog box which can be dragged / resized

    var status = new dockspawn.PanelContainer($("#status_window")[0], dockManager);

    var editorDiv = $("#editor1_window")[0],
        editor = new JSONEditor(editorDiv);

    editor.set({
      'array': [1, 2, 3],
      'boolean': true,
      'null': null,
      'number': 123,
      'object': {'a': 'b', 'c': 'd'},
      'string': 'Hello World'
    });    

    var editor1 = new dockspawn.PanelContainer(editorDiv, dockManager),
        noise = new dockspawn.PanelContainer($("#noise_window")[0], dockManager),
        render = new dockspawn.PanelContainer($("#render_window")[0], dockManager);

    var documentNode = dockManager.context.model.documentManagerNode,
        statusNode = dockManager.dockDown(documentNode, status, 0.1),
        renderNode = dockManager.dockFill(documentNode, render),
        noiseNode = dockManager.dockLeft(renderNode, noise, 0.25),
        editor1Node = dockManager.dockUp(noiseNode, editor1, 0.5);
});

