// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


// Prevent drag and drop from opening a json as a new web page.
define( ['jquery', 'noiseTree', 'status', 'client', 'file'],
  function($, tree, status, client, file) {
    console.log("events loading...");    
    // console.log("   $", $);    
    // console.log("   editor", editor);    
    // console.log("   noiseTree", tree);    
    // console.log("   status", status);    
    // console.log("   client", client);    

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

