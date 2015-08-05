(function($){
	$(document).ready(function() {

	//object to handle new content creation
	var newContent = null;
	var base_path = '';
	var lfz = {};

	if(Drupal.settings && Drupal.settings.basePath){
		base_path = Drupal.settings.basePath;
	}

	if(Drupal.settings && Drupal.settings.lfz){
		lfz = Drupal.settings.lfz;
	}

	//load agenda items on document load
	var url = base_path+'ajax/today/agenda-items';
	if(lfz && lfz.date_search_format){
		url += '/'+lfz.date_search_format;
	}

	var dataObj = {};

	if(lfz && lfz.class_nid){
		dataObj.class_nid = lfz.class_nid;
	}

    $.ajax({
    	url: url,
                type:'GET',
                data:dataObj,
                dataType:'json',
                success:function(response){
                	if(response['success'] && response['content']){
                		addAgendaItem(response['content']);
                		$('#search-results').html('');
                	}else{
                		var noDataContent = $('<h4>').html("No Agenda Items for today");
                		$('#todays_agenda').html(noDataContent);
                	}
                }
    });

    //bind key up to a function to call a search function
    $('#edit-search').keyup(function(event){
    	searchAddItem.apply(this, event);

    });
    $('#edit-search').focus(searchAddItem);
    //when outside the input field wait 500 milliseconds before emptying the search results
    $('#edit-search').blur(function(event){
    	setTimeout(function(){
    		$("#search-results").html('');
    	}, 500);
    })

    /**
    *	Click Handlers
    */

    //search result click handler
    $('body').on('click', '.add-item-search-result', function() {
        var nid = $(this).data('nid');
        if (nid) {

        	var url = base_path+'ajax/today/add-item';
			if(lfz && lfz.date_search_format){
				url += '/'+lfz.date_search_format;
			}

			var dataObj = {
				'nid':nid
			};

			if(lfz && lfz.class_nid){
				dataObj.class_nid = lfz.class_nid;
			}

            $.ajax({
                url: url,
                data: dataObj,
                type:'GET',
                dataType:'json',
                success:function(response){
                	if(response['success'] && response['content']){
                		addAgendaItem(response['content']);	
                		$('#search-results').html('');
                	}else{
                		console.error("No data returned check ajax/today/add-item url");
                	}
                }
            });
        }
    });

    //when searching the results give you the ability to create new agenda item
    // below we are creating the click handler for that specific element
    $('body').on('click', '.add-new-item', function(){
    	var newItemTitle = $(this).data('search-val');
    	newContent = {
    		title:newItemTitle,
    		type:null
    	};

    	getAgendaItemTypes().then(function(response){
    		if(response.success){

    			$('#agendaModal').find('.modal-title').html('New Content - '+newItemTitle);
    			$('#agendaModal').find('.modal-body .content').html('<h3>Select Type</h3>');
    			$('#agendaModal').find('.modal-body .content').append(response.content);
    			$('#agendaModal').modal();
    		}else{
    			console.error("Error getting agenda item types. response : ", response);
    		}
    	}).fail(function(response){
    		console.error("Error getting agenda item types. response : ", response);
    	});
    });

    //add click event handler for item type selection
	$('body').on('click', '.agenda-item-types', function(){
		var type = $(this).data('type');
		if(type){
			newContent.type = type;

			if(newContent.title != null && newContent.type != null){
				//create alert to show that we are trying to save
				var saveAlert = createAlert('Attempting to save new Content - '+newContent.title, 'warning');
				addAlertTo(saveAlert, $('#agendaModal').find('.alert-con'));

				var url = base_path+'ajax/today/add-new-item';
				if(lfz && lfz.date_search_format){
					url += '/'+lfz.date_search_format;
				}

				if(lfz && lfz.class_nid){
					newContent.class_nid = lfz.class_nid;
				}

				$.ajax({
		            url: url,
		            data: newContent,
		            type:'GET',
		            dataType:'json',
		            success:function(response){
	            		//let the user know that it was saved
		            	if(response['success'] && response['content']){
		            		//add saved alert
							addAlertTo(createAlert('New Content Saved - '+newContent.title, 'success'),
								$('#agendaModal').find('.alert-con'));

		            		addAgendaItem(response['content']);		            		

		            		setTimeout(function(){
		            			newItemCreateComplete();
		            		}, 1000);
		            	}else{
		            		//add error alert
							addAlertTo(createAlert('Unable to Save - '+newContent.title, 'danger'),
								$('#agendaModal').find('.alert-con'));
		            	}
		            	
		            },
		            error:function(response){
						addAlertTo(createAlert('Unable to Save - '+newContent.title, 'danger'),
							$('#agendaModal').find('.alert-con'));
		            }
		        });
			}else{
				console.error("newContent object doesnt have any content in it");
			}

		}else{
			console.error("No Type was selected, shouldnt be here");
		}
	});
	
	//remove agenda item click handler
	$('body').one('click', '.remove-agenda-item', removeAgendaItem);

	/**
    *	Click Handlers End
    */

	function newItemCreateComplete(){

		newContent.title = null;
		newContent.type = null;
		//hide modal
		$('#agendaModal').modal('hide');

		//clear alerts for future
		$('#agendaModal').find('.alert-con').html('');
	}

	function createAlert(content, type){
		if(!type){
			type = 'success';
		}
		var alert = $('<div>').addClass('alert').addClass('alert-'+type).html(content);

		return alert;
	}

	function addAlertTo(alertElm, selector, replace){
		if(replace === undefined || replace === null){
			replace = true;
		}

		if(replace){
			$(selector).find('.alert').remove();
		}
		$(selector).append(alertElm);
	}

    function getAgendaItemTypes(){
    	return $.ajax({
            url: base_path+'ajax/agenda-item-types',
            type:'GET',
            dataType:'json'
        });
    }

    function searchAddItem(){
    	var elm = $(this);
    	//use timeout variable to not have the search happen with every letter typed
    	var to = $(elm).data('to');

    	if(!to){
    		var to = setTimeout(function(){
    			var currentVal = $(elm).val();
		    	if(currentVal.length > 1){
		    		//provide closure so that the search val doesnt change for the AJAX request
		    		(function(localVal){
		    			var url = base_path+'ajax/search/'+localVal;
			    		$.ajax({
			                url: url,
			                dataType:'json',
			                success:function(response){
			                	if(response.success){
			                		$("#search-results").html('');
			                		$("#search-results").html(response.content);
			                	}else{
			                		console.error("No data returned check search");
			                	}
			                }
			            });
		    		})(currentVal);    		
		    	}
		    	//clear timeout
		    	$(elm).data('to',null);
    		}, 250);

    		//set timeout variable
    		$(elm).data('to', to);
    	}
    }

    function createAgendItemDomElm(obj){
    	if(!obj){
    		return $('<div>No Agenda Items For Today</div>');
    	}

    	obj.urlLink = 'node/'+obj.nid;
    	var itemCon = $('<div>').addClass('list-group-item agenda-item').attr('id', 'nid-'+obj.nid).data(obj);
    	var iconItem = $('<span>');
    	var titleItem = $('<h5>').addClass('col-sm-9');
    	var typeItem = $('<small>').html(obj.type);
    	titleItem.append(obj.title+' - ', typeItem);
    	var delBtn = $('<input type="button">').addClass('btn btn-danger col-sm-3 pull-right').val('Remove');
    	itemCon.append(iconItem, titleItem, delBtn, '<span class="clearfix"></span>');

    	itemCon.click(function(event){
    		var urlLink = $(this).data('urlLink');
    		if(urlLink){
    			window.open(urlLink, "_blank");
    			return;
    		}
    	});

    	delBtn.click(removeAgendaItem);

    	return itemCon;
    }

    function addAgendaItem(html){
   	
    	$('#todays_agenda').find('h4').remove();
    	$('#todays_agenda').append(html);
    }

    function removeAgendaItem(event){

		var listItem = $(this).parents('.agenda-list-item');

		(function(elm){
			var item_nid = elm.data('nid');
			if(!item_nid){
	    		console.error('Cant remove agenda item with no data-nid');
	    		return;
	    	}
    		var index = elm.index();
    		var agenda_nid = elm.parent('.list-group').data('agenda-nid');
    		var date = null;
    		if(lfz && lfz.date_search_format){
				date = lfz.date_search_format;
			}

    		var url = base_path+'ajax/remove-agenda-item';

    		var dataObj = {
	            	'agenda_nid':agenda_nid,
	                'item_nid': item_nid,
	                'index':index,
	                'date':date
	            };

	        if(lfz && lfz.class_nid){
	        	dataObj.class_nid = lfz.class_nid;
	        }

			$.ajax({
	            url: url,
	            data: dataObj,
	            type:'POST',
	            dataType:'json',
	            success:function(response){
	            	if(response.success){
	            		elm.remove();
	            		$('#search-results').html('');
	            	}else{
	            		elm.addClass('list-group-item-danger');
	            	}
	            },
	            error:function(response){
	            	elm.addClass('list-group-item-danger');
	            }
	        });

		})(listItem)
    }
});
})(jQuery)
