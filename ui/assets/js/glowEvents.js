// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


requirejs( ['jquery', 'docker', 'editor', 'noiseTree', 'status', 'client'],
  function($, docker, editor, tree, status, client) {
    console.log("Events loaded...", docker);    

    // Prevent drag and drop from opening a json as a new web page.
    $(document).on('dragenter', function (e) {
        e.stopPropagation();
        e.preventDefault();
    });

    $(document).on('dragover', function (e) {
      e.stopPropagation();
      e.preventDefault();
      // e.target.css('border', '2px dotted #0B85A1');
    });

    $(document).on('drop', function (e) {
        e.stopPropagation();
        e.preventDefault();

        var evt = e.originalEvent,
            f = evt.dataTransfer.files[0],            
            reader = new FileReader();

        console.log("Reading file: ", f);

        status.append("Reading file " + f.name + ".");

        reader.onload = function(e) {
          console.log("Finished reading file: ", e);              
          status.append("Finished reading " + f.name + " (" + e.loaded + " bytes).");

          var t = e.currentTarget.result,
              j = JSON.parse(t);

          editor.editor.set(j);
          tree.parseJSON(j);
          client.request('glow/setScene', j);
        };

        reader.readAsText(f); 
    });

    docker.manager.addLayoutListener({
      onDock: function(mgr, dockNode){
        console.log("docking ", dockNode);
        if (docker.noiseNode.name == dockNode.name) {
          tree.refresh();
        }
      },

      onUndock: function(mgr, dockNode){
        console.log("undocking ", dockNode);
        if (docker.noiseNode.name == dockNode.name) {
          tree.refresh();
        }
      },

      onSuspendLayout: function(mgr){
        // console.log("onSuspendLayout ");
        // tree.refresh();
      },

      onResumeLayout: function(mgr){
        // console.log("onResumeLayout ");
        tree.refresh();
      }

    });

    tree.cy.bind('select', function(evt){
      // console.log('select', evt);
      if (!_.has(evt, 'cyTarget') ||
          !_.has(evt.cyTarget, '_private') ||
          !_.has(evt.cyTarget._private, 'ids') )
        return;

      var ids = _.keys(evt.cyTarget._private.ids);
      if (ids.length <= 0 )
        return;

      // console.log("ids", ids);
      // console.log("editor", editor);
      var results = editor.editor.search(ids[0]),
          result = _.find(results, function(r){
            return r.node.getValue() == ids[0];
          });

      // console.log("results", results);
      // console.log("result", result);
      if (result) {
        editor.editor.search("");
        editor.editor.collapseAll();
        result.node.scrollTo();
      }

    });    

  }
);

