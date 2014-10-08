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

                    start: function(e, ui, widget) {
                        // console.log("START:\n", e, ui, widget);
                    },

                    resize: function(e, ui, widget) {
                        // console.log("RESIZE:\n", e, ui, widget);
                        events.resizing(widget);
                    },

                    stop: function(e, ui, widget) {
                        var gridster    = $(".gridster ul").gridster().data('gridster'),
                            glows       = $(".gridster li").find(".glow-window"),
                            cells       = _.map(glows, function(g) { // Capture everything needed to restore a cell...
                                            var p = $(g).parent(); 
                                            return { 
                                                id:     $(g).attr("id"), 
                                                col:    $(p).attr("data-col"),
                                                row:    $(p).attr("data-row"),
                                                size_x: $(p).attr("data-sizex"),
                                                size_y: $(p).attr("data-sizey")
                                            }; 
                                        }),
                            js          = JSON.stringify(cells);

                        console.log("cells: ", js);
                        // console.log("STOP:\n", e, ui, widget);
                        events.resizeStopped(widget);

                        // Gridster erroneously assumes that none of the widgets will have moved.
                        // Get all of the glow windows, get their names, then 
                        //      from their parent cell obtain the position and size
                        // serialize all three of these instead.
                        // cells:  [{"id":"editor1_window","col":"1","row":"1","size_x":"2","size_y":"2"},{"id":"noise_window","col":"3","row":"1","size_x":"2","size_y":"2"},{"id":"tool_window","col":"5","row":"1","size_x":"3","size_y":"1"},{"id":"render_window","col":"8","row":"2","size_x":"12","size_y":"7"},{"id":"log_window","col":"8","row":"1","size_x":"3","size_y":"1"}] 

                        localStorage.setItem('glowGrid', js);                        
                    }
                }
            }).data("gridster"),
            lastUserInputWasBackspace = false

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

        // Serialization not working!

        // console.log("gridster:", gridster);
        // console.log("grid:", grid);
        // console.log("grid:", grid.serialize());

        // Find the parent cell of the serialized id, and set the size appropriately.
        gridSerialized = localStorage.getItem('glowGrid');
        if (gridSerialized && gridSerialized.length) {
            gridJson = JSON.parse(gridSerialized);
            if (gridJson && gridJson.length) {
                console.log("Serialized grid: ", gridJson);
                _.each(gridJson, function(c){
                    var cell = $("#" + c.id).parent();
                    // gridster.resize_widget(cell, c.size_x, c.size_y);
                    // gridster.mutate_widget_in_gridmap(cell, c.coords().grid, cell);
                });
            }
            // gridJson = gridster.sort_by_row_and_col_asc(gridJson);


            // $.each(serialization, function() {
            //     grid.add_widget('<li />', this.size_x, this.size_y, this.col, this.row);
            // });            
        }

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
