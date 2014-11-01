// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that is responsible for the render window. 


// Prevent drag and drop from opening a json as a new web page.
define(['jquery', 'underscore', 'observer', 'jquery_ui', 'jquery_tiles'],
    function($, _, observer) {
        // Resizes the parent cell of the render image to fit the image.
        var resizeCell = function() {
            var gridster = $(".gridster ul").gridster().data('gridster'),
                cell = $("#render_window").parent(),
                img = $("#renderImage"),
                cellWidth = parseInt(cell.attr('data-sizex')),
                cellHeight = parseInt(cell.attr('data-sizey')),
                deltaWidth = img.width() - cell.width(),
                deltaCellWidth = deltaWidth > 0 ? parseInt(((deltaWidth / gridster.min_widget_width) + 1).toFixed()) : 0,
                newCellWidth = cellWidth + deltaCellWidth,
                deltaHeight = img.height() - cell.height(),
                deltaCellHeight = deltaHeight > 0 ? parseInt(((deltaHeight / gridster.min_widget_height) + 1).toFixed()) : 0,
                newCellHeight = cellHeight + deltaCellHeight;

            gridster.resize_widget(cell, newCellWidth, newCellHeight);
        };

        console.log("render loading...");

        // observer.subscribe("renderScene", function(evt) {
        //     var img = $("#renderImage");
        //     img.attr("src", "data:image/png;base64," + evt.image);

        //     resizeCell();

        //     img.show();
        //     $("#renderBusy").hide();
        // });

        observer.subscribe("newScene", function(evt) {
            // Tell the tiles to start requesting new renders.
            $('#tiledRenderImage').tiles('zoom', 1);
            $("#renderImage").hide();
        });


        $('#tiledRenderImage').tiles({
            original: {
                width: 2048,
                height: 2048,
            },
            basePath: "glow/render/",
            loading: "images/load.gif", // Placeholder image for loading images
            zoom: 0, // Show the original content (i.e. the splash screen)
            tileSize: 256
        });

        $('#tiledRenderImage').tiles("center");

        $('#tiledRenderImage').dblclick(function(e) {
            console.log("dblclick", e);

            // "tiles-1-1-1"
            var targetClass = $(e.target).attr("class");
            if (targetClass == undefined){
                return;
            }

            var fields      = targetClass.split("-"),
                zoom        = fields[1],
                col         = fields[2],
                row         = fields[3],
                x           = ((parseInt(col) - 1) * 256) + 128,
                y           = ((parseInt(row) - 1) * 256) + 128,
                cx          = $("#tiledRenderImage").width() / 2.0,
                cy          = $("#tiledRenderImage").height() / 2.0,
                dx          = Math.abs(x - cx),
                dy          = Math.abs(y - cy);

            console.log("col " + col + " row " + row);
            console.log("x " + x + " y " + y);

            if (x < cx && (x - dx < 0)) {
                console.log("-X Edge ", x - dx);
            }
            if (x >= cx && (x + dx > 2048)) {
                console.log("+X Edge ", x + dx - 2048);
            }
            if (y < cy && (y - dy < 0)) {
                console.log("-Y Edge ", y - dy);
            }
            if (y >= cy && (y + dy > 2048)) {
                console.log("+Y Edge ", y + dy - 2048);
            }

            $('#tiledRenderImage').tiles("move", x, y);
        });


        return {
            resizeCell: resizeCell
        };
    }
);