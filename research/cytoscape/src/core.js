;(function($$, window){ 'use strict';

  var isTouch = $$.is.touch();

  var defaults = {
    hideEdgesOnViewport: false
  };
  
  var origDefaults = $$.util.copy( defaults );

  $$.defaults = function( opts ){
    defaults = $$.util.extend({}, origDefaults, opts);
  };

  $$.fn.core = function( fnMap, options ){
    for( var name in fnMap ){
      var fn = fnMap[name];
      $$.Core.prototype[ name ] = fn;
    }
  };
  
  $$.Core = function( opts ){
    if( !(this instanceof $$.Core) ){
      return new $$.Core(opts);
    }
    var cy = this;

    opts = $$.util.extend({}, defaults, opts);

    var container = opts.container;
    var reg = container ? container._cyreg : null; // e.g. already registered some info (e.g. readies) via jquery
    reg = reg || {};

    if( reg && reg.cy ){ 
      if( container ){ container.innerHTML = ''; }
      reg.cy.notify({ type: 'destroy' }); // destroy the renderer

      reg = {}; // old instance => replace reg completely
    }

    var readies = reg.readies = reg.readies || [];
    
    if( container ){ container._cyreg = reg; } // make sure container assoc'd reg points to this cy
    reg.cy = cy;

    var options = opts;
    options.layout = $$.util.extend( { name: window && container ? 'grid' : 'null' }, options.layout );
    options.renderer = $$.util.extend( { name: window && container ? 'canvas' : 'null' }, options.renderer );
    
    // TODO determine whether we need a check like this even though we allow running headless now
    // 
    // if( !$$.is.domElement(options.container) ){
    //   $$.util.error("Cytoscape.js must be called on an element");
    //   return;
    // }
    
    var _p = this._private = {
      ready: false, // whether ready has been triggered
      initrender: false, // has initrender has been triggered
      options: options, // cached options
      elements: [], // array of elements
      id2index: {}, // element id => index in elements array
      listeners: [], // list of listeners
      aniEles: [], // array of elements being animated
      scratch: {}, // scratch object for core
      layout: null,
      renderer: null,
      notificationsEnabled: true, // whether notifications are sent to the renderer
      minZoom: 1e-50,
      maxZoom: 1e50,
      zoomingEnabled: options.zoomingEnabled === undefined ? true : options.zoomingEnabled,
      userZoomingEnabled: options.userZoomingEnabled === undefined ? true : options.userZoomingEnabled,
      panningEnabled: options.panningEnabled === undefined ? true : options.panningEnabled,
      userPanningEnabled: options.userPanningEnabled === undefined ? true : options.userPanningEnabled,
      boxSelectionEnabled: options.boxSelectionEnabled === undefined ? true : options.boxSelectionEnabled,
      autolockNodes: false,
      autoungrabifyNodes: options.autoungrabifyNodes === undefined ? false : options.autoungrabifyNodes,
      zoom: $$.is.number(options.zoom) ? options.zoom : 1,
      pan: {
        x: $$.is.plainObject(options.pan) && $$.is.number(options.pan.x) ? options.pan.x : 0,
        y: $$.is.plainObject(options.pan) && $$.is.number(options.pan.y) ? options.pan.y : 0
      },
      hasCompoundNodes: false
    };

    // set selection type
    var selType = options.selectionType;
    if( selType === undefined || (selType !== 'additive' && selType !== 'single') ){
      // then set default

      if( isTouch ){
        _p.selectionType = 'additive';
      } else {
        _p.selectionType = 'single';
      }
    } else {
      _p.selectionType = selType;
    }

    // init zoom bounds
    if( $$.is.number(options.minZoom) && $$.is.number(options.maxZoom) && options.minZoom < options.maxZoom ){
      _p.minZoom = options.minZoom;
      _p.maxZoom = options.maxZoom;
    } else if( $$.is.number(options.minZoom) && options.maxZoom === undefined ){
      _p.minZoom = options.minZoom;
    } else if( $$.is.number(options.maxZoom) && options.minZoom === undefined ){
      _p.maxZoom = options.maxZoom;
    }

    // init style

    if( $$.is.stylesheet(options.style) ){
      _p.style = options.style.generateStyle(this);
    } else if( $$.is.array(options.style) ) {
      _p.style = $$.style.fromJson(this, options.style);
    } else if( $$.is.string(options.style) ){
      _p.style = $$.style.fromString(this, options.style);
    } else {
      _p.style = new $$.Style( cy );
    }

    // create the renderer
    cy.initRenderer( $$.util.extend({
      hideEdgesOnViewport: options.hideEdgesOnViewport,
      hideLabelsOnViewport: options.hideLabelsOnViewport,
      textureOnViewport: options.textureOnViewport
    }, options.renderer) );

    // trigger the passed function for the `initrender` event
    if( options.initrender ){
      cy.on('initrender', options.initrender);
      cy.on('initrender', function(){
        cy._private.initrender = true;
      });
    }

    // initial load
    cy.load(options.elements, function(){ // onready
      cy.startAnimationLoop();
      cy._private.ready = true;

      // if a ready callback is specified as an option, the bind it
      if( $$.is.fn( options.ready ) ){
        cy.bind('ready', options.ready);
      }

      // bind all the ready handlers registered before creating this instance
      for( var i = 0; i < readies.length; i++ ){
        var fn = readies[i];
        cy.bind('ready', fn);
      }
      if( reg ){ reg.readies = []; } // clear b/c we've bound them all and don't want to keep it around in case a new core uses the same div etc
      
      cy.trigger('ready');
    }, options.done);
  };

  $$.corefn = $$.Core.prototype; // short alias
  

  $$.fn.core({
    ready: function(){
      return this._private.ready;
    },

    initrender: function(){
      return this._private.initrender;
    },

    destroy: function(){
      this.notify({ type: 'destroy' }); // destroy the renderer

      var domEle = this.container();
      var parEle = domEle.parentNode;
      if( parEle ){
        parEle.removeChild( domEle );
      }

      return this;
    },

    getElementById: function( id ){
      var index = this._private.id2index[ id ];
      if( index !== undefined ){
        return this._private.elements[ index ];
      }

      // worst case, return an empty collection
      return new $$.Collection( this );
    },

    selectionType: function(){
      return this._private.selectionType;
    },

    hasCompoundNodes: function(){
      return this._private.hasCompoundNodes;
    },

    addToPool: function( eles ){
      var elements = this._private.elements;
      var id2index = this._private.id2index;

      for( var i = 0; i < eles.length; i++ ){
        var ele = eles[i];

        var id = ele._private.data.id;
        var index = id2index[ id ];
        var alreadyInPool = index !== undefined;

        if( !alreadyInPool ){
          index = elements.length;
          elements.push( ele );
          id2index[ id ] = index;
          ele._private.index = index;
        }
      }

      return this; // chaining
    },

    removeFromPool: function( eles ){
      var elements = this._private.elements;
      var id2index = this._private.id2index;

      for( var i = 0; i < eles.length; i++ ){
        var ele = eles[i];

        var id = ele._private.data.id;
        var index = id2index[ id ];
        var inPool = index !== undefined;

        if( inPool ){
          delete this._private.id2index[ id ];
          elements.splice(index, 1);

          // adjust the index of all elements past this index
          for( var j = index; j < elements.length; j++ ){
            var jid = elements[j]._private.data.id;
            id2index[ jid ]--;
          }
        }
      }
    },

    container: function(){
      return this._private.options.container;
    },

    options: function(){
      return $$.util.copy( this._private.options );
    },
    
    json: function(params){
      var json = {};
      var cy = this;
      
      json.elements = {};
      cy.elements().each(function(i, ele){
        var group = ele.group();
        
        if( !json.elements[group] ){
          json.elements[group] = [];
        }
        
        json.elements[group].push( ele.json() );
      });

      json.style = cy.style().json();
      json.scratch = cy.scratch();
      json.zoomingEnabled = cy._private.zoomingEnabled;
      json.userZoomingEnabled = cy._private.userZoomingEnabled;
      json.zoom = cy._private.zoom;
      json.minZoom = cy._private.minZoom;
      json.maxZoom = cy._private.maxZoom;
      json.panningEnabled = cy._private.panningEnabled;
      json.userPanningEnabled = cy._private.userPanningEnabled;
      json.pan = cy._private.pan;
      json.boxSelectionEnabled = cy._private.boxSelectionEnabled;
      json.layout = cy._private.options.layout;
      json.renderer = cy._private.options.renderer;
      json.hideEdgesOnViewport = cy._private.options.hideEdgesOnViewport;
      json.hideLabelsOnViewport = cy._private.options.hideLabelsOnViewport;
      json.textureOnViewport = cy._private.options.textureOnViewport;
      
      return json;
    }
    
  });  
  
})( cytoscape, typeof window === 'undefined' ? null : window );
