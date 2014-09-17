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
        jquery_ui: 'jquery-ui.min',
        dockspawn: 'dockspawn'
    }
});

requirejs(['jquery', 'jquery_ui', 'dockspawn'],
function($) {
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
    var properties = new dockspawn.PanelContainer($("#properties_window")[0], dockManager);
    var problems = new dockspawn.PanelContainer($("#problems_window")[0], dockManager);
    var editor1 = new dockspawn.PanelContainer($("#editor1_window")[0], dockManager);
    var outline = new dockspawn.PanelContainer($("#outline_window")[0], dockManager);
    var render = new dockspawn.PanelContainer($("#render_window")[0], dockManager);

    // Dock the panels on the dock manager
    var documentNode = dockManager.context.model.documentManagerNode;

    var propertiesNode = dockManager.dockLeft(documentNode, properties, 0.6);
    var problemsNode = dockManager.dockRight(documentNode, problems, 0.40);
    var editor1Node = dockManager.dockFill(documentNode, editor1);
    var outlineNode = dockManager.dockDown(documentNode, outline, 0.2);
    var renderNode = dockManager.dockFill(documentNode, render);
});

