// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


// Prevent drag and drop from opening a json as a new web page.
define(['jquery', 'status', 'underscore', 'observer', 'jquery_ui'],
    function($, log, _, observer) {

        var request = function(opts) {
            var msg_ = _.has(opts, 'msg') ? opts.msg : '{}',
                text = JSON.stringify(msg_),
                cb_ = _.has(opts, 'callback') ? opts.callback : function() {};

            log.append("> " + opts.url + " : " + text);

            $.ajax({
                contentType: 'application/json; charset=utf-8',
                data: text,
                dataType: 'json',
                url: opts.url,
                success: function(data, status, jqXhr) {
                    console.log("data:", data);
                    console.log("status:", status);
                    console.log("jqXhr:", jqXhr);

                    var t = JSON.stringify(data);
                    log.append("< " + t);
                    cb_(data, status, jqXhr);
                },

                error: function(jqXhr, status, thrown) {
                    console.log("jqXhr:", jqXhr);
                    console.log("status:", status);
                    console.log("thrown:", thrown);

                    var response = jqXhr.statusText,
                        dlg = $("#dialog");

                    console.log("response:", response);

                    dlg.text(response);
                    dlg.dialog({
                        title: "Error from server!",
                        modal: true,
                        buttons: {
                            Ok: function() {
                                $(this).dialog("close");
                            }
                        }
                    });

                    log.append("< " + status);
                }
            });
        };

        console.log("observer loading...");

        observer.subscribe("requestNewScene", function(evt) {
            request({
                'url': "scenes/default.json",
                'callback': function(data, status, jqXhr) {
                    console.log(data);
                    console.log(status);
                    console.log(jqXhr);
                    if (jqXhr.status == 200) {
                        observer.sendEvent({
                            name: "newScene",
                            scene: data
                        });
                    }
                }
            });
        });

        observer.subscribe("newScene", function(evt) {
            request({
                'url': "glow/preview",
                'msg': evt.scene,

                'callback': function(data, status, jqXhr) {
                    console.log(data);
                    console.log(status);
                    console.log(jqXhr);
                }
            });
        });

        observer.subscribe("requestRenderScence", function(evt) {
            request({
                'url': "glow/render",
                'msg': evt.scene,

                'callback': function(data, status, jqXhr) {
                    console.log(data);
                    console.log(status);
                    console.log(jqXhr);
                    if (jqXhr.status == 200) {
                        observer.sendEvent({
                            name:   "renderScene",
                            image:  data.image
                        });                      
                    }
                }
            });
        });
    }
);