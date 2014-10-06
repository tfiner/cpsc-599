// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that is responsible for the render window. 


// Prevent drag and drop from opening a json as a new web page.
define(['jquery', 'underscore', 'jquery_ui'],
    function($, _) {
        console.log("render loading...");

        return {

            resizeCell: function() {
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
            },

            setImage: function(options) {
                if (_.has(options, 'image')) {
                    // Set the returned image.
                    $("#renderImage").attr("src", "data:image/png;base64," + options.image);

                    this.resizeCell();

                    img.show();
                    $("#renderBusy").hide();
                }
            }
        };
    }
);