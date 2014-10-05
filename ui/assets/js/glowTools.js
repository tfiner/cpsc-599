// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


// Prevent drag and drop from opening a json as a new web page.
define(['jquery', 'file', 'jquery_ui'],
    function($, file) {
        console.log("tools loading...");

        $("#open").button({
            icons: {
                primary: "icon-folder-open"
            }
        }).click(function(e) {
            console.log("clicked open");
            e.preventDefault();
            $('#fileInput').click();
        });

        $('#fileInput').on('change', function(evt) {
          // e.stopPropagation();
          // e.preventDefault();

          var f = evt.target.files[0];
          file.read(f);
        });

        $("#save").button({
            icons: {
                primary: "icon-save"
            }
        });

        $("#render").button({
            icons: {
                primary: "icon-camera"
            }
        });
    });