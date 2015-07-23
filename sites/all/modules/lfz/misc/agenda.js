(function($){
	$(document).ready(function(){

		var base_path = '';
		var lfz = {};
		var mouseIsDown = false;

		if(Drupal.settings && Drupal.settings.basePath){
			base_path = Drupal.settings.basePath;
		}

		if(Drupal.settings && Drupal.settings.lfz){
			lfz = Drupal.settings.lfz;
		}

		$('.sortable').sortable({
			connectWith: ".connected-sortable",
		}).on('sort', sortHandler);

		function sortHandler(event, ui){

			//events get called too quickly to act on so we set a timeout to handle once
			// the user stops sorting
			var to = ui.item.data('to');
			
			if(to && mouseIsDown){
				//clear timeout
				clearTimeout(to);
				ui.item.data('to', false);
			}

			ui.item.data('to', setTimeout(function(item){
				if(mouseIsDown){
					sortHandler.apply(this, [event, ui]);
				}else{
					//wait till mouse is no longer down
					saveAgendaItem(item);
				}
			}, 500, ui.item));
		}

		$('body').on('keydown',function(evt){
			//meta key
			if(evt.keyCode == 91 ||
				evt.keyCode == 17){
				$('.agenda-list-item').css('cursor', 'cell');
			}
		});

		$('body').on('keyup',function(evt){
			if(evt.keyCode == 91 ||
				evt.keyCode == 17){
				$('.agenda-list-item').css('cursor', 'auto');
			}
		});

		$(document).on('contextmenu', function(evt){
			if(evt.ctrlKey){
				evt.preventDefault();
			}
		});

		$('body').on('mousedown', '.agenda-list-item',function(evt){
			if(evt.metaKey ||
				evt.ctrlKey){
				var newElm = $(this).clone();
				newElm.addClass('clone-agenda-list-item');
				newElm.data('clone', 1);
				var offset = $(this).offset();
				$(this).parent('.sortable').append(newElm);
				$(this).parent('.sortable').sortable('refresh');

				saveAgendaItem(newElm);
			}

			mouseIsDown = true;
		});


		$('body').on('mouseup', function(evt){
			mouseIsDown = false;
		});

		//remove agenda item click handler
		$('body').on('click', '.remove-agenda-item', removeAgendaItem);

        //add skill assessment results button handler
        $('body').on('click', '.add-sa-results', function(){
            var listItem = $(this).parent('.agenda-list-item');

            console.log("listItem : ", listItem);
            var saResultsInputCon = $('<div>').addClass("results-form-con");
            var textArea = $('<textarea rows="7" placeholder="Insert CSV results">').addClass("col-xs-12");
            var saveButton = $('<input type="button">').addClass('btn btn-success').val("Save");
            $(saveButton).on('click', saveSkillAssessmentResultsButtonClick);
            var cancelButton = $('<input type="button">').addClass('btn btn-default').val("Cancel");
            $(cancelButton).on('click', cancelSkillAssessmentResultsButtonClick);
            saResultsInputCon.append(textArea, saveButton, cancelButton);
            listItem.append(saResultsInputCon);

            $(this).hide();
        });

        function saveSkillAssessmentResultsButtonClick(){

            var listItem = $(this).parents('.agenda-list-item');
            var agenda_nid = listItem.parent('.list-group').data('agenda-nid');
            var nid = listItem.data('nid');
            var insertedValue = $(this).parent().find('textarea').val();

            if(insertedValue != "" && nid){

                var url = base_path+'ajax/skillassessment/save-results';

                (function(clickedElm){
                    var alertElm = $('<div class="alert" role="alert">');
                    //also would need to be changed inside agenda-item-list template
                    var results_url = base_path+"skillassessment/details/"+agenda_nid;
                    var resultsButton = $('<a>').addClass('btn btn-sm btn-default pull-right').html("View Results").attr({'href':results_url, 'target':'blank'})
                    var dataObj = {
                        data:insertedValue,
                        resource_nid:nid,
                        agenda_nid:agenda_nid
                    };
                    $.ajax({
                        url: url,
                        data: dataObj,
                        type:'POST',
                        dataType:'json',
                        success:function(response){
                            console.log(response);
                            if(response.success){
                                alertElm.addClass('alert-success').html("Results Saved Successfully");
                                clickedElm.parents('.agenda-list-item').append(resultsButton)
                            }else{
                                alertElm.addClass('alert-danger').html("Couldn't save results for Skill Assessment");
                            }
                        },
                        error:function(response){
                            alertElm.addClass('alert-danger').html("Couldn't save results for Skill Assessment");
                        }
                    }).always(function(){
                        clickedElm.parents('.agenda-list-item').append(alertElm);
                        clickedElm.parents('.agenda-list-item').find('.results-form-con').remove();
                    });
                })($(this));



            }else{
                //cant submit empty result
                console.error("User must insert value into textarea before attempting to save");
            }
        }

        function cancelSkillAssessmentResultsButtonClick(){
            $(this).parents('.agenda-list-item').find('.add-sa-results').show();
            $(this).parents('.agenda-list-item').find('.results-form-con').remove();
        }

		function removeAgendaItem(event){

			var listItem = $(this).parent('.agenda-list-item');

			(function(elm){
				var item_nid = listItem.data('nid');
				if(!item_nid){
		    		console.error('Cant remove agenda item with no data-nid');
		    		return;
		    	}
	    		var index = listItem.index();
	    		var agenda_nid = listItem.parent('.list-group').data('agenda-nid');
	    		var date = null;
	    		if(lfz && lfz.date_search_format){
					date = lfz.date_search_format;
				}
	    		

	    		var url = base_path+'ajax/remove-agenda-item';
	    		elm.addClass('list-group-item-warning');

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

		function saveAgendaItem(item){
			var item_nid = item.data('nid');
			var current_agenda_id = item.data('agenda-nid');
			var new_agenda_id =  item.parent('.list-group').data('agenda-nid');
			var agenda_date = item.parent('.list-group').data('date');
			var clone = item.data('clone');
			var current_index = item.data('index');
			var new_index = item.index();

			var obj = {
				item_nid : item_nid,
				current_agenda_id : current_agenda_id,
				new_agenda_id : new_agenda_id,
				new_index:new_index,
				current_index: current_index,
				agenda_date:agenda_date,
				clone:clone
			};

			if(lfz && lfz.class_nid){
				obj.class_nid = lfz.class_nid;
			}

			if(current_agenda_id == new_agenda_id
				&& current_index == new_index){
				//item is exactly where we started dragging from
				return;
			}


			function updateItemProperties(elm){
				var ajax_obj = elm.data('ajax-obj');
				elm.data('clone', 0);
				elm.data('index', ajax_obj.new_index);
				if(ajax_obj.new_agenda_id){
					elm.data('agenda-nid', ajax_obj.new_agenda_id);
				}else{
					elm.data('agenda-nid', false);
				}
			}

			item.data('ajax-obj', obj);
			item.addClass('list-group-item-warning');

			(function(elm){
				var url = base_path+'ajax/agenda-item-save';
				$.ajax({
					url:url,
					data:obj,
					dataType:'json',
					success:function(response){
						if(response.success){
							elm.addClass('list-group-item-success');
							setTimeout(function(){
								elm.removeClass('list-group-item-success');
							}, 5000);
							updateItemProperties(elm);
						}else{
							elm.addClass('list-group-item-danger');
						}
						elm.removeClass('list-group-item-warning');
					},
					error:function(response){
						elm.removeClass('list-group-item-warning');
						elm.addClass('list-group-item-danger');
					}
				});
			})(item);
		}
	});
})(jQuery)