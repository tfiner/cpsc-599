;(function($$){ 'use strict';

  var CanvasRenderer = $$('renderer', 'canvas');

  CanvasRenderer.prototype.createBuffer = function(w, h) {
    var buffer = document.createElement('canvas');
    buffer.width = w;
    buffer.height = h;
    
    return [buffer, buffer.getContext('2d')];
  };

  CanvasRenderer.prototype.bufferCanvasImage = function( options ){
    var data = this.data;
    var cy = data.cy;
    var bb = cy.boundingBox();
    var width = options.full ? Math.ceil(bb.w) : this.data.container.clientWidth;
    var height = options.full ? Math.ceil(bb.h) : this.data.container.clientHeight;
    var scale = 1;

    if( options.full && options.scale !== undefined ){
      width *= options.scale;
      height *= options.scale;

      scale = options.scale;
    }

    var buffCanvas = document.createElement('canvas');

    buffCanvas.width = width;
    buffCanvas.height = height;

    buffCanvas.style.width = width + 'px';
    buffCanvas.style.height = height + 'px';

    var buffCxt = buffCanvas.getContext('2d');

    // Rasterize the layers, but only if container has nonzero size
    if (width > 0 && height > 0) {

      buffCxt.clearRect( 0, 0, width, height );

      if( options.bg ){
        buffCxt.fillStyle = options.bg;
        buffCxt.rect( 0, 0, width, height );
        buffCxt.fill();
      }

      buffCxt.globalCompositeOperation = 'source-over';

      if( options.full ){ // draw the full bounds of the graph
        this.redraw({
          forcedContext: buffCxt,
          drawAllLayers: true,
          forcedZoom: scale,
          forcedPan: { x: -bb.x1, y: -bb.y1 },
          forcedPxRatio: 1
        });
      } else { // draw the current view
        this.redraw({
          forcedContext: buffCxt,
          drawAllLayers: true,
          forcedZoom: cy.zoom(),
          forcedPan: cy.pan(),
          forcedPxRatio: 1
        });
      }
    }

    return buffCanvas;
  }; 

  CanvasRenderer.prototype.png = function( options ){
    return this.bufferCanvasImage( options ).toDataURL('image/png');
  };

})( cytoscape );