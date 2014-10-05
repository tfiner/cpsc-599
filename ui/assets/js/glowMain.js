// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that defines the main interface.
//


define(
    ['jquery', 'gridster', 'status', 'events'],

    function($, gridster, status, events) {
        console.log("glowMain loading...");
        console.log("   jquery", $);
        console.log("   gridster", gridster);
        console.log("   status", status);
        console.log("   events", events);

        // Turn off scroll bars.
        // $("body").css("overflow", "hidden");
        // document.documentElement.style.overflow = 'hidden';

        // Turn on tooltips, ugh these are annoying 
        // (don't forget to include jquery_ui if using this again...)
        // $( document ).tooltip();


        var grid = $(".gridster ul").gridster({
            widget_margins: [10, 10],
            widget_base_dimensions: [140, 140],
            helper: 'clone',
            resize: {
                enabled: true
            },
            draggable: {
                handle: 'header'
            },
            resize: {
                enabled: true,
                start: function(e, ui, widget) {
                    // console.log("START:\n", e, ui, widget);
                },

                resize: function(e, ui, widget) {
                    // console.log("RESIZE:\n", e, ui, widget);
                    events.resizing(widget);
                },

                stop: function(e, ui, widget) {
                    // console.log("STOP:\n", e, ui, widget);
                    events.resizeStopped(widget);
                }
            }
        });

        return {
            grid: grid
        }
    }
);
