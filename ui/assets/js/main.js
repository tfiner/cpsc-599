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
        jquery_layout: 'jquery.layout-latest'
    }
});

requirejs(['jquery', 'jquery_ui', 'jquery_layout'],
function($) {
    console.log("All loaded.");
    $('body').layout({ 
        applyDefaultStyles: true,
        west__paneSelector: "#west",
        east__paneSelector: "#east"         
    });
});

