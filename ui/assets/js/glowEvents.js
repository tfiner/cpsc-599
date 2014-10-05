// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


// Prevent drag and drop from opening a json as a new web page.
define( ['jquery', 'editor', 'noiseTree', 'status', 'client', 'file'],
  function($, editor, tree, status, client, file) {
    console.log("events loading...");    
    console.log("   $", $);    
    console.log("   editor", editor);    
    console.log("   noiseTree", tree);    
    console.log("   status", status);    
    console.log("   client", client);    

    $(document).on('dragenter', function (e) {
        e.stopPropagation();
        e.preventDefault();
    });

    $(document).on('dragover', function (e) {
      e.stopPropagation();
      e.preventDefault();
      // e.target.css('border', '2px dotted #0B85A1');
    });

    // Load a dropped JSON as a new scene file.
    $(document).on('drop', function (e) {
        // Prevent the browser from thinking it needs to show the dropped file as source.
        e.stopPropagation();
        e.preventDefault();

        var evt = e.originalEvent,
            f = evt.dataTransfer.files[0];

        file.read(f);
    });

    // Use cytoscapes select event to select the corresponding
    // node in the json editor.
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

    var resizeGlow = function(widget) {
      // console.log("resizing: ", widget);
      var newHeight = widget.height() - widget.children("header").height() * 2,
          noise = widget.children("#noise_window");

      // Set all of the glow window childen to match the parent.
      widget.children(".glow-window").height(newHeight);
      widget.children(".glow-window").width(widget.width());

      // If this is the noise window, tell it to refresh its canvas.
      if (noise.length) {
        tree.refresh();
      }
    };

    return {

      resizeStopped: function(widget) {
        var gridSerialized;
        // console.log("resizeStopped:\n", widget);

        if (!_.has(widget, "length")) {
          return;
        }

        resizeGlow(widget);
      },

      resizing: function(widget) {
        // console.log("resizing:\n", widget);

        if (!_.has(widget, "length")){
          return;
        }

        resizeGlow(widget);
      }
    }    

  }
);

