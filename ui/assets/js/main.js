// This file contains the main javascript for the interface.

require.config({
    baseUrl: 'js',
    paths: {
        // the left side is the module ID,
        // the right side is the path to
        // the jQuery file, relative to baseUrl.
        // Also, the path should NOT include
        // the '.js' file extension.
        jquery:         'jquery-1.11.1.min',
        jquery_ui:      'jquery-ui.min',
        jquery_tiles:   'jquery.tiles',
        gridster:       'jquery.gridster.min',
        jsoneditor:     'jsoneditor.min',
        cytoscape:      'cytoscape.min',
        underscore:     'underscore-min',

    shim: {
            'md5': {
                exports: 'CryptoJS'
            },
            'enc-base64-min': {
                exports: 'CryptoJS'
            }
        },
        events:     'glowEvents',
        glowMain:   'glowMain',
        file:       'glowFile',
        editor:     'glowEditor',
        noiseTree:  'glowNoiseTree',
        status:     'glowStatus',
        client:     'glowClient',
        tools:      'glowTools',
        observer:   'glowObserver',
        render:     'glowRender'
    }
});


requirejs(
    ['glowMain'],
    function() {
        console.log("glowMain requested.");

    }
);
