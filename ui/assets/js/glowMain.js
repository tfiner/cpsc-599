// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that defines the main interface.
//


define(
    ['jquery', 'gridster', 'status', 'events', 'tools'],

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

        var i,
            gridSerialized,
            gridJson,
            panes = $(".gridster li"), 
            grid = $(".gridster ul").gridster({
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

                        // Not working!
                        // Save the current workspace.
                        // gridSerialized = gridster.serialize();
                        // localStorage.setItem('glowGrid', JSON.stringify(gridSerialized));                        
                    }
                }
            });

        // Serialization not working!
        // console.log("gridster:", gridster);
        // console.log("grid:", grid);
        // console.log("grid:", grid.serialize());

        // gridSerialized = localStorage.getItem('glowGrid');
        // if (gridSerialized && gridSerialized.length) {
        //     gridJson = JSON.parse(gridSerialized);
        //     if (gridJson && gridJson.length) {
        //         console.log("Serialized grid: ", gridJson);
        //     }
            // gridJson = gridster.sort_by_row_and_col_asc(gridJson);

            // grid.remove_all_widgets();
            // $.each(serialization, function() {
            //     grid.add_widget('<li />', this.size_x, this.size_y, this.col, this.row);
            // });            
        // }

        // Start everything off in sync with gridster parent divs.
        console.log("Forcing a resize...");
        for(i = 0; i < panes.length; ++i)
            events.resizing($(panes[i]));

    }
);
