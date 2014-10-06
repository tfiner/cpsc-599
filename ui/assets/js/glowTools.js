// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that is responsible for the tools window. 


// Prevent drag and drop from opening a json as a new web page.
define(['jquery', 'file', 'client', 'render', 'jquery_ui'],
    function($, file, client, render) {
        console.log("tools loading...");

        var saveJson = function() {
            var json = '{}',
                jsonData = 'data:application/json;charset=utf-8,' + JSON.stringify(json);

            $(this).attr({
                'href': jsonData,
                'target': '_blank'
            });
        };

        $("#open").button({
            icons: {
                primary: "icon-folder-open"
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
        });

        $("#save").button({
            icons: {
                primary: "icon-save"
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
            icons: {
                primary: "icon-camera"
            }
        }).click(function(e) {
            $("#renderImage").hide();
            $("#renderBusy").show();

            client.request({
              'url':"glow/render",
              'callback': function(data, status, jqXhr){
                console.log(data);
                console.log(status);
                console.log(jqXhr);
                if (jqXhr.status == 200) {
                  render.setImage(data);
                }
              }               
            });
        });
    });