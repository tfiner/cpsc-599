// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file is the module that handles JS events and routes them 
// to the other modules.


// Prevent drag and drop from opening a json as a new web page.
define( ['jquery', 'status', 'jquery_ui'],
  function($, log) {
    console.log("Client loaded...");   


    return {
      request: function(url, msg, callback) {
        var msg_ = msg || '{}',
            text = JSON.stringify(msg_),
            cb_ = callback || function(){};

        log.append("> " + url + " : " + text);

        $.ajax({
          contentType: 'application/json; charset=utf-8',
          data: text,          
          dataType: 'json',
          url: url,
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

              var response = jqXhr.statusText;
              console.log("response:", response);

              $( "#dialog" ).text(response);
              $( "#dialog" ).dialog({
                title: "Error from server!",
                modal: true,
                buttons: {
                  Ok: function() {
                    $( this ).dialog( "close" );
                  }
                }                  
              });

              log.append("< " + status);
          }
        });

      }
    }
  }
);

