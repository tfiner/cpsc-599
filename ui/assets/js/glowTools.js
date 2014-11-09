// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that is responsible for the tools window. 


define(['jquery', 'file', 'observer', 'jquery_ui'],
    function($, file, observer) {
        console.log("tools loading...");

        var saveJson = function() {
            var json = '{}',
                jsonData = 'data:application/json;charset=utf-8,' + JSON.stringify(json);

            $(this).attr({
                'href': jsonData,
                'target': '_blank'
            });
        };

        $("#new").button({
            text: false,
            icons: {
                primary: "fa fa-file"
            }
        }).click(function(e) {
            e.preventDefault();
            observer.sendEvent({name:"requestNewScene"});
        });

        $("#open").button({
            text: false,
            icons: {
                primary: "fa fa-folder-open"
            }
        }).click(function(e) {
            // Forward the click of this button to a hidden file input control.
            // Note that this is hack, because there isn't file open mechanism
            // available to DOM/JS code that doesn't use an Input form element.
            console.log("clicked open");
            e.preventDefault();
            $('#fileInput').click();
        });

        // Catch the event where the fileInput hidden control is changed,
        // call our own JSON file reader.
        $('#fileInput').on('change', function(evt) {
            var f = evt.target.files[0];
            file.read(f);
            $('#fileInput').val("");
        });

        $("#save").button({
            text: false,
            icons: {
                primary: "fa fa-save"
            }
        }).click(function(e) {
            // Forward the click of this button to a hidden link.
            console.log("clicked save");
            e.preventDefault();

            var onOk = function() {
                var fsa = $('#fileSaveAs'),
                    fname = $("#dialogSaveAs").find('input[name="fileName"]').val();

                console.log("fname", fname);
                fsa.attr("download", fname);
                fsa.get(0).click();
            };

            var dlg = $("#dialogSaveAs").dialog({
                resizable: false,
                closeOnEscape: true,
                height: 200,
                modal: true,
                buttons: {
                    Ok: function() {
                        $(this).dialog("close");
                        onOk();
                    }
                }
            }).keyup(function(e) {
                if (e.keyCode == 13) {
                    $(this).dialog("close");
                    onOk();
                }
            });

        });

        $("#render").button({
            text: false,
            icons: {
                primary: "fa fa-camera"
            }
        }).click(function(e) {
            observer.sendEvent({
                name:   "renderEditScene"
            });
        });

    });