<?php
$role = isset($content['#lfz']['show_role']) ? $content['#lfz']['show_role'] : '';
$show_user_thumb = isset($content['#lfz']['show_user_thumb']) ? $content['#lfz']['show_user_thumb'] : false;
$show_comment_form = isset($content['#lfz']['show_comment_form']) ? $content['#lfz']['show_comment_form'] : false;
?>
<?php
if ($content['comment_form'] && $show_comment_form): ?>
    <div class="col-sm-12">
      <button class="btn btn-primary pull-right" data-toggle="collapse" data-target="#resource-comment-con"><?php print t('Add Comment');?></button>
      <div class="clear-fix"></div>
      <div class="collapse well col-xs-12" id="resource-comment-con">
        <?php print render($content['comment_form']);?>
      </div>
    </div>
  <?php
endif;?>
<div id="comments" class="col-xs-12 <?php print $classes;?>"<?php print $attributes;?>>
<!-- <?php
if ($content['comments'] && $node->type != 'forum'): ?>
   <?php
print render($title_prefix);?>
    <h2 class="title"><?php
print t('Comments');?></h2>
    <?php
print render($title_suffix);?>
  <?php
endif;?> -->

  <?php
foreach ($content['comments'] as $comment):
    if (!is_array($comment) || !array_key_exists('comment_body', $comment)) {
        continue;
    }
    $comment_node = $comment['comment_body']['#object'];
    $comment_text = $comment_node->comment_body['und'][0]['value'];
    $user = user_load($comment_node->uid);

    //only show comments depending on the role passed in
    if (!user_has_role(array_search($role, user_roles()), $user)) {
        continue;
    }

    $first_name = field_get_items('user', $user, 'field_first_name');
    $first_name_output = field_view_value('user', $user, 'field_first_name', $first_name[0]);

    $last_name = field_get_items('user', $user, 'field_last_name');
    $last_name_output = field_view_value('user', $user, 'field_last_name', $last_name[0]);

    // print_r(render($output));
    // $name = ($user->field_first_name['und'][0]['value'])
    // print_r($user);
    if ($user->picture && $user->picture->uri) {
        $uri = $user->picture->uri;
    }
    $img = (isset($uri)) ? file_create_url($uri) : base_path() . drupal_get_path('module', 'lfz') . '/assets/avatar_2x.png';
    ?>
								<div class="row">
								    <?php
    if ($show_user_thumb):
    ?>
								    <div class="col-xs-2 <?php
    print($comment_node->pid != 0) ? 'col-sm-offset-1' : '';?>">
								      <div class="thumbnail">
								        <img class="img-responsive user-photo" src="<?php
    print $img;?>">
								      </div><!-- /thumbnail -->
								    </div><!-- /col-sm-1 -->
								    <div class="col-xs-9">
								      <div class="panel panel-default">
								        <div class="panel-heading">
								          <strong><?php
    print render($first_name_output) . ' ' . render($last_name_output);?></strong> <span class="text-muted">commented <?php
    print _time_elapsed_string($comment_node->changed);?></span>
								        </div>
								        <div class="panel-body">
								          <?php
    print $comment_text;?>
								        </div><!-- /panel-body -->
								      </div><!-- /panel panel-default -->
								    </div><!-- /col-sm-5 -->
								  </div>
								    <?php
    else:
    ?>
								      <div class="col-xs-12">
								      <div class="panel panel-default">
								        <div class="panel-heading">
								          <strong><?php print render($first_name_output) . ' ' . render($last_name_output);?></strong>
		                      <span class="text-muted">commented <?php print _time_elapsed_string($comment_node->changed);?></span>
								        </div>
								        <div class="panel-body">
								          <?php print $comment_text;?>
								        </div><!-- /panel-body -->
								      </div><!-- /panel panel-default -->
								    </div><!-- /col-sm-5 -->
								  </div>
								      <?php
    endif;
    ?>

								<?php
endforeach;
?>


</div>