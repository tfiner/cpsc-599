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
        jq_resize:  'jquery.resize'
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
    ['jquery', 'cytoscape'],

    function($, cytoscape) {
        console.log("Cytoscape loaded...");

        window.cy = cytoscape({
          container: $('#noise_window')[0],

          style: 
            cytoscape.stylesheet()
            .selector('node')
              .css({
                'content': 'data(name)',
                'text-valign': 'center',
                'color': 'white',
                'text-outline-width': 2,
                'text-outline-color': '#888'
              })
            .selector('edge')
              .css({
                'target-arrow-shape': 'triangle'
              })
            .selector(':selected')
              .css({
                'background-color': 'black',
                'line-color': 'black',
                'target-arrow-color': 'black',
                'source-arrow-color': 'black'
              })
            .selector('.faded')
              .css({
                'opacity': 0.25,
                'text-opacity': 0
              }),
          
          elements: {
            nodes: [
              { data: { id: 'j', name: 'Jerry' } },
              { data: { id: 'e', name: 'Elaine' } },
              { data: { id: 'k', name: 'Kramer' } },
              { data: { id: 'g', name: 'George' } }
            ],
            edges: [
              { data: { source: 'j', target: 'e' } },
              { data: { source: 'j', target: 'k' } },
              { data: { source: 'j', target: 'g' } },
              { data: { source: 'e', target: 'j' } },
              { data: { source: 'e', target: 'k' } },
              { data: { source: 'k', target: 'j' } },
              { data: { source: 'k', target: 'e' } },
              { data: { source: 'k', target: 'g' } },
              { data: { source: 'g', target: 'j' } }
            ]
          },
          
          layout: {
            name: 'cose',
            padding: 10
          },
          
          ready: function(){
            console.log("cytoscape ready...");
//            window.cy = this;
          }

        });     
    }
);

requirejs(
    ['jquery', 'jsoneditor'],

    function($, JSONEditor) {
        console.log("Editor loaded.");

        var editorDiv = $("#editor1_window")[0],
            editor = new JSONEditor(editorDiv);

        // Drag and drop support for the json editor.
        editorDiv.addEventListener(
          'dragover', 
          function(evt){
            evt.stopPropagation();
            evt.preventDefault();
            evt.dataTransfer.dropEffect = 'copy';
          }, 
          false
        );

        editorDiv.addEventListener(
          'drop', 
          function(evt){
            evt.stopPropagation();
            evt.preventDefault();

            var f = evt.dataTransfer.files[0],            
                reader = new FileReader();

            console.log("Reading file: ", f);

            reader.onload = function(e) {
              console.log("Finished reading file: ", e);              

              var t = e.currentTarget.result,
                  j = JSON.parse(t);

              editor.set(j);
            };

            reader.readAsText(f); 
          }, 
          false
        );

        editor.set({
          'msg': 'Drag and drop a glow scene JSON file here.'
        });      
    }
);


requirejs(
    ['jquery', 'jq_resize'],

    function($, cytoscape) {
      console.log('Resizer loaded.');

      $('#noise_window').resize(function(e) { 
        // console.log(e); 
        if(window.cy)
          window.cy.resize();
          window.cy.fit();
      });
    }
);

