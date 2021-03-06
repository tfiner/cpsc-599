// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


// Prevent drag and drop from opening a json as a new web page.
define( ['jquery', 'status', 'observer'],
    function($, status, observer) {

        console.log("file loading...");

        return {

            read: function(f, numBytes) {
                var reader = new FileReader();

                status.append("Reading file " + f.name + ".");

                reader.onload = function(e) {
                    console.log("Finished reading file: ", e);
                    status.append("Finished reading " + f.name + " (" + e.loaded + " bytes).");

                    var t = e.currentTarget.result,
                        j = JSON.parse(t);

                    observer.sendEvent({
                        name:"newScene",
                        scene:j
                    });
                };

                reader.readAsText(f);
            }
        }

    });