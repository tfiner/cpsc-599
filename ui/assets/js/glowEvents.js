// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


// Prevent drag and drop from opening a json as a new web page.
requirejs( ['jquery', 'editor', 'noiseTree'],
  function($, glowEditor, glowNoiseTree) {
    console.log("Events loaded...", glowEditor, glowNoiseTree);    

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
        console.log(e);
        e.stopPropagation();
        e.preventDefault();

        var evt = e.originalEvent,
            f = evt.dataTransfer.files[0],            
            reader = new FileReader();

        console.log("Reading file: ", f);

        reader.onload = function(e) {
          console.log("Finished reading file: ", e);              

          var t = e.currentTarget.result,
              j = JSON.parse(t);

          glowEditor.editor.set(j);
          glowNoiseTree.parseJSON(j);
        };

        reader.readAsText(f); 
    });

  }
);

