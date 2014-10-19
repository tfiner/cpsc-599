// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


define(
    ['jquery', 'cytoscape', 'underscore'],

    function($, cytoscape, _) {
        console.log("cytoscape loading...");

        $("#noise_new").button({
            text: false,            
            icons: {
                primary: "icon-plus"
            }
        }).click(function(e) {
            
        });


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
                    // .selector('.faded')
                    // .css({
                    //     'opacity': 0.25,
                    //     'text-opacity': 0
                    // }),

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
        };

        // console.log(tree);

        // $('#noise_window').resize(function(e) {
        //     // console.log(e);
        //     tree.refresh(); 
        // });

        console.log("cytoscape loaded.");

        return tree;
    }
);