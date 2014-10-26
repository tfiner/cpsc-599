// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


define(
    ['jquery', 'cytoscape', 'underscore', 'observer'],

    function($, cytoscape, _, observer) {
        var tree = {
                cy: cytoscape({
                    container: $('#noise_window')[0],

                    style: cytoscape.stylesheet()
                        .selector('node')
                        .css({
                            'content': 'data(name)',
                            'text-valign': 'center',
                            'color': 'black',
                            'text-outline-width': 2,
                            'text-outline-color': 'white',
                            'height': 256,
                            'width': 256,
                            'shape': 'roundrectangle',
                            'font-size': 36
                        })
                        .selector('edge')
                        .css({
                            'width': 16,
                            'target-arrow-shape': 'triangle',
                            'line-color': 'black',
                            'target-arrow-color': 'black'
                        })
                        .selector(':selected')
                        .css({
                            'border-width': 8,
                            'border-color': '#ff0000'
                        }),

                    layout: {
                        name: 'breadthfirst',
                        directed: true,
                        padding: 10
                    },

                    ready: function() {                        
                        console.log("cytoscape ready...");                                                
                    }
                }),

                // Given a glow JSON scene, turn it into nodes and edges that 
                // cytoscape can render.
                parseJSON: function(j) {
                    console.log("Parse: ", j);
                    console.log("Tree: ", tree);

                    tree.cy.remove(tree.cy.elements());
                    tree.cy.resize();

                    if (!_.has(j, 'objects')) {
                        console.error("JSON missing objects array, ignoring.");
                        return;
                    }

                    if (!_.isArray(j.objects) || j.objects.length == 0) {
                        console.error("JSON 'objects' is not an array, or doesn't have any elements, ignoring.");
                        return;
                    }

                    console.log("objs: ", j.objects.length, j.objects);
                    if (!_.has(j.objects[0], 'functions') || !_.isArray(j.objects[0].functions)) {
                        console.error("JSON missing objects[0].functions array, ignoring.");
                        return;
                    }

                    var funcs = j.objects[0].functions;
                    var nodes = _.map(funcs, function(fn) {
                        return {
                            data: {
                                id: fn.name,
                                name: fn.name,
                            }
                        };
                    });
                    console.log("Nodes: ", nodes);

                    var edges = [];
                    _.each(["source0", "source1", "control"],
                        function(srcName) {
                            var targets = _.filter(funcs, srcName);
                            var e = _.map(targets, function(t) {
                                return {
                                    data: {
                                        source: t[srcName],
                                        target: t.name
                                    }
                                };
                            });
                            edges = edges.concat(e);
                        }
                    );
                    console.log("edges: ", edges);

                    tree.cy.load({
                        nodes: nodes,
                        edges: edges
                    });

                    console.log("loaded:", tree.cy.nodes());
                    _.each(tree.cy.nodes(), function(ele) {
                        console.log("set image to :",
                            window.location.href + 'glow/noise/' + ele.data("id") + ".png");

                        ele.css('background-image',
                            window.location.href + 'glow/noise/' + ele.data("id") + ".png");
                    });
                },

                refresh: function() {
                    // console.log("refresh tree.");
                    tree.cy.resize();
                    tree.cy.fit();
                }
            },
            
            perlinTypes = {
                "frequency":    1.0,
                "lacunarity":   2.0,
                "octave":       6.0,
                "quality":      1.0,
                "seed":         0.0 
            },

            noiseTypes = {
                "perlin":       {name: "Perlin", params: _.extend(perlinTypes, {"persistence":0.5}) },
                "billow":       {name: "Billow", params: _.extend(perlinTypes, {"persistence":0.5}) },
                "ridgedmulti":  {name: "Ridged", params: perlinTypes},
                "scale":        {name: "Scale",  params: {"scale":1.0, "bias":0.0}},
                "select":       {name: "Select", params: {"edgeFalloff":0.0, "lower":-1.0, "upper":1.0}} 
            },

            setNoiseType = function(noiseType) {
                var noise       = _.find(noiseTypes, 
                        function(v, k){
                            return k == noiseType;
                        }),
                    typeParams  = $('#typeParams');

                $('#typeParams p').remove();

                _.each(noise.params, function(defaultVal, name){
                    typeParams
                        .append($('<p>')
                            .append($('<label>').text(name))
                            .append($('<input>', {
                                type:   "number", 
                                name:   name, 
                                id:     name, 
                                value:  defaultVal,
                                class:  "glow-input"
                            }))
                        );
                    });
            },

            onOk = function() {
                // Build the JSON for a new libnoise node.
                var noiseNode   = { 'lib':  'libnoise', 
                                    'type': $('#nodeType option:selected').attr('name') },
                    treeNode;

                // Gather and append the dialog's inputs:
                $.each($('#nodeEditor input'), 
                    function(k,v){ 
                        noiseNode[v.name] = v.value; 
                    });

                observer.sendEvent({name:'newNoiseNode', node:noiseNode});

                treeNode    = { group: 'nodes',
                                data: { 
                                    id: noiseNode.name,  
                                    name: noiseNode.name
                                }
                            };

                console.log("treeNode", treeNode);
                tree.cy.add(treeNode);
                tree.cy.layout();
            };            

        $("#noise_new").button({
            text: false,            
            icons: {
                primary: "fa fa-plus"
            }
        }).click(function(e) {
            var nodeType = $("#nodeType");

            $("#nodeType option").remove();

            $.each(noiseTypes, function(key, value) {   
                var name = value["name"];
                nodeType
                    .append($('<option>', { name : key })
                    .text(name)); 
            });

            $("#nodeType").change(function(e){
                var type = $('#nodeType option:selected').attr('name');
                setNoiseType(type);
            });

            $("#nodeEditor").dialog({
                closeOnEscape: true,
                height: 500,
                width: 400,
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

            setNoiseType("perlin");
        });

        $("#noise_delete").button({
            text: false,            
            icons: {
                primary: "fa fa-minus"
            }
        }).click(function(e) {
            tree.cy.elements(":selected").remove();
        });

        $("#noise_link").button({
            text: false,            
            icons: {
                primary: "fa fa-link"
            }
        }).click(function(e) {
            $("#nodeEditor").dialog({
                title: "Link Nodes",
                closeOnEscape: true,
                height: 500,
                width: 400,
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

        $("#noise_unlink").button({
            text: false,            
            icons: {
                primary: "fa fa-unlink"
            }
        }).click(function(e) {
        });

        // Node selection
        tree.cy.on('tap', 'node', function(evt) {
            // console.log('select', evt);
            if (!_.has(evt, 'cyTarget') ||
                !_.has(evt.cyTarget, '_private') ||
                !_.has(evt.cyTarget._private, 'ids'))
                return;

            var ids = _.keys(evt.cyTarget._private.ids);
            if (ids.length <= 0)
                return;

            observer.sendEvent({
                name: "treeSelectNode",
                id: ids[0]
            });
        });

        // Edge selection
        tree.cy.on('tap', function(evt) {
            console.log('tap', evt);
            // TODO:1 Fix selection for edges.
            evt.cy.elements(":selected").css({'line-color':'red'})
            // Doesn't do what I think it does...
            // evt.cy.forceRender();
        });

        observer.subscribe("newScene", function(evt) {
            tree.parseJSON(evt.scene);
        });


        // Turns off zoom while scrolling, this prevents the case where a user
        // is scrolling the window, and ends up zooming.
        (function() {        
            var timer;
            $(window).bind('scroll',function () {
                clearTimeout(timer);
                timer = setTimeout( refresh , 150 );
                tree.cy.zoomingEnabled(false);
            });
            var refresh = function () { 
                console.log('Stopped scrolling.'); 
                tree.cy.zoomingEnabled(true);
            };
        })();


        // $('#noise_window').bind("contextmenu",function(e){
        //     console.log("clicked");
        // });

        console.log("cytoscape loaded.", tree);

        return tree;
    }
);