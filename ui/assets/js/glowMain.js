// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that defines the main interface.
//


define(
    ['jquery', 'gridster', 'status', 'events', 'render', 'underscore', 'tools'],

    function($, gridster, status, events, render, _) {
        console.log("glowMain loading...");
        // console.log("   jquery", $);
        // console.log("   gridster", gridster);
        // console.log("   status", status);
        // console.log("   events", events);

        // Turn off scroll bars.
        // $("body").css("overflow", "hidden");
        // document.documentElement.style.overflow = 'hidden';

        // Turn on tooltips, ugh these are annoying 
        // (don't forget to include jquery_ui if using this again...)
        // $( document ).tooltip();

        var i,
            gridSerialized, 
            gridJson,
            panes = $(".gridster li"), 
            gridster = $(".gridster ul").gridster({
                widget_margins: [10, 10],
                widget_base_dimensions: [64, 64],
                // autogrow_cols: true,
                extra_cols: 10,
                extra_rows: 10,
                helper: 'clone',
                draggable: {
                    handle: 'header'
                },
                resize: {
                    enabled: true,

                    resize: function(e, ui, widget) {
                        events.resizing(widget);
                    }
                }
            }).data("gridster"),
            lastUserInputWasBackspace = false;

        // This hackery prevents the retarded behavior of backspace 
        // meaning navigate back a page.
        // http://stackoverflow.com/a/22949859/196682
        $(document).keydown(function(event){
            lastUserInputWasBackspace = event.which == 8;
        })
        $(document).mousedown(function(){
            lastUserInputWasBackspace = false;
        })
        $(window).on('beforeunload', function(){
            if (lastUserInputWasBackspace) {
                return "Are you sure you want to leave this page?";
            }
        })

        // Start everything off in sync with gridster parent divs.
        console.log("Forcing a resize...");
        for(i = 0; i < panes.length; ++i)
            events.resizing($(panes[i]));

        render.resizeCell();

        return { 
            gridster: gridster 
        };

    }
);
