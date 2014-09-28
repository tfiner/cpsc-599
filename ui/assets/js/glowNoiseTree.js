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

                parseJSON: function(j) {
                    console.log("Parse: ", j);
                    
                },

            })
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