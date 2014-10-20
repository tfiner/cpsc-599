// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


// Prevent drag and drop from opening a json as a new web page.
define(["underscore"],
    function(_) {
        // Stored as { "eventName": [observer1, observer2, ...] }
        var observers = {};

        console.log("observer loading...");

        var observer = {

            subscribe: function(evt, observer) {
                if (!_.has(observers,evt)) {
                    observers[evt] = [];
                }

                var dupe = _.find(observers[evt], function(o){return o.prototype == observer.prototype;});
                if (dupe == undefined) {
                    observers[evt].push(observer);
                }
            },

            sendEvent: function(evt) {
                if (_.has(observers,evt)){
                    _.each(observers[evt], function(o){ 
                        o(evt); 
                    });
                }
            },

            unSubscribe: function(evt) {
                console.log(observers);
                if (_.has(observers,evt)) {
                    observers[evt] = []; 
                }
                console.log(observers);                
            }
        }

        console.log(observer);
        return observer;
    }
);