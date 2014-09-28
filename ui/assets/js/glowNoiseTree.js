// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


define(
    ['jquery', 'cytoscape', 'underscore', 'jq_resize'],

    function($, cytoscape, _) {
        console.log("Cytoscape loaded...");

        var tree = {
            cy: cytoscape({
                container: $('#noise_window')[0],

                style: cytoscape.stylesheet()
                    .selector('node')
                    .css({
                        'content': 'data(name)',
                        'text-valign': 'center',
                        'color': 'white',
                        'text-outline-width': 2,
                        'text-outline-color': '#888'
                    })
                    .selector('edge')
                    .css({
                        'target-arrow-shape': 'triangle'
                    })
                    .selector(':selected')
                    .css({
                        'background-color': 'black',
                        'line-color': 'black',
                        'target-arrow-color': 'black',
                        'source-arrow-color': 'black'
                    })
                    .selector('.faded')
                    .css({
                        'opacity': 0.25,
                        'text-opacity': 0
                    }),

                elements: {
                    nodes: [{
                        data: {
                            id: 'j',
                            name: 'Jerry'
                        }
                    }, {
                        data: {
                            id: 'e',
                            name: 'Elaine'
                        }
                    }, {
                        data: {
                            id: 'k',
                            name: 'Kramer'
                        }
                    }, {
                        data: {
                            id: 'g',
                            name: 'George'
                        }
                    }],
                    edges: [{
                        data: {
                            source: 'j',
                            target: 'e'
                        }
                    }, {
                        data: {
                            source: 'j',
                            target: 'k'
                        }
                    }, {
                        data: {
                            source: 'j',
                            target: 'g'
                        }
                    }, {
                        data: {
                            source: 'e',
                            target: 'j'
                        }
                    }, {
                        data: {
                            source: 'e',
                            target: 'k'
                        }
                    }, {
                        data: {
                            source: 'k',
                            target: 'j'
                        }
                    }, {
                        data: {
                            source: 'k',
                            target: 'e'
                        }
                    }, {
                        data: {
                            source: 'k',
                            target: 'g'
                        }
                    }, {
                        data: {
                            source: 'g',
                            target: 'j'
                        }
                    }]
                },

                layout: {
                    name: 'cose',
                    padding: 10
                },

                ready: function() {
                    console.log("cytoscape ready...");
                }
            }),

            parseJSON: function(j) {
                console.log("Parse: ", j);
                console.log("Now: ", _.now());
                console.log("Tree: ", tree);

                tree.cy.remove( tree.cy.elements() );
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
                    function(srcName){
                        console.log(srcName, funcs);
                        var targets = _.filter(funcs, srcName);
                        var e = _.map(targets, function(t) { 
                            return {
                                data: {
                                    source: t[srcName],
                                    target: t.name
                                }
                            }; 
                        });
                        console.log("e: ", e);
                        edges = edges.concat(e);
                    }
                );
                console.log("edges: ", edges);

                tree.cy.load({
                    nodes: nodes,
                    edges: edges
                });
            }
        };

        console.log(tree);

        $('#noise_window').resize(function(e) {
            // console.log(e); 
            tree.cy.resize();
            tree.cy.fit();
        });

        return tree;
    }
);