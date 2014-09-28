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
        underscore: 'underscore-min',

        docker:     'glowDock',
        editor:     'glowEditor',
        noiseTree:  'glowNoiseTree',
        events:     'glowEvents',
        status:     'glowStatus',
        client:     'glowClient'
    }
});


requirejs(
    ['docker'],
    function() {}
);


requirejs(
    ['events'],
    function() {}
);

requirejs(
    ['status'],
    function() {}
);

requirejs(
    ['client'],
    function(client) {
        client.request('server/status')
    }
);

