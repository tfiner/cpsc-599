// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


// Prevent drag and drop from opening a json as a new web page.
define( ['jquery'],
function($) {
    console.log("Status loaded...");

    var div = $("#log_window");

    return {
        append: function(msg) {
            div.append(msg + "\n");
        }
    }
});

