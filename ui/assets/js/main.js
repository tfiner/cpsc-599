// This file contains the main javascript for the interface.

require.config({
    baseUrl: 'js',
    paths: {
        // the left side is the module ID,
        // the right side is the path to
        // the jQuery file, relative to baseUrl.
        // Also, the path should NOT include
        // the '.js' file extension.
        jquery: 'jquery-1.11.1.min',
        jquery_ui: 'jquery-ui.min'
    }
});

// require(['jquery'], function( $ ) {
//     console.log( $ ) // OK
// });


// Start the main app logic.
requirejs(['jquery', 'jquery_ui'],
function($) {
    var pane1 = $(".resizable1");

    console.log("All loaded.");
    console.log(pane1);

    pane1.resizable(
    {
        autoHide: true,
        handles: 'e',
        resize: function(e, ui) 
        {
            var parent = ui.element.parent();
            var remainingSpace = parent.width() - ui.element.outerWidth(),
                divTwo = ui.element.next(),
                divTwoWidth = (remainingSpace - (divTwo.outerWidth() - divTwo.width()))/parent.width()*100+"%";
                divTwo.width(divTwoWidth);
        },
        stop: function(e, ui) 
        {
            var parent = ui.element.parent();
            ui.element.css(
            {
                width: ui.element.width()/parent.width()*100+"%",
            });
        }
    });

});

