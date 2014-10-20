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

        return {

            subscribe: function(evtName, observer) {
                if (!_.has(observers,evtName)) {
                    observers[evtName] = [];
                }

                var dupe = _.find(observers[evtName], function(o){return o.prototype == observer.prototype;});
                if (dupe == undefined) {
                    observers[evtName].push(observer);
                }
            },

            sendEvent: function(evt) {
                if (_.has(observers,evt.name)){
                    _.each(observers[evt.name], function(o){ 
                        o(evt); 
                    });
                }
            },

            unSubscribe: function(evtName) {
                if (_.has(observers,evtName)) {
                    observers[evtName] = []; 
                }
            }
        }

    }
);