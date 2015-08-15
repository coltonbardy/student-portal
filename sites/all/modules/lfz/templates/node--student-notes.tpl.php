<?php

$links = $field_related_resources;
$skills = $field_related_skills;
$student = $field_student_reference_link;
$comments = $body;
$resource_links = $links[0]['node']->field_instructor_reference_link['und'][0];
$student_name = $student[0]['node']->title;
$note_author = $student[0]['node']->name;
$resource = $skills[0]['node']->title;
echo '<pre>';
print_r($resource);
echo '</pre>';

?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> row clearfix"<?php print $attributes; ?>>
    <div class="col-xs-8">
        <div class="row">
            <div class="col-xs-12">
                <div class="panel panel-default">
                    <div class="panel-heading">
                        <h3 class="panel-title" style="display: inline;">Student: <?php echo(l($student_name,'node/' . $student[0]['nid'])) ?></h3>
                        <h3 class="panel-title" style="display: inline; float: right;">Note Author: <?php echo(l($note_author,'user/' . $variables['uid'])) ?></h3>
                    </div>
                    <div class="panel-body">
                        <div class="row" style="padding: 15px;">
                            <?php
                                foreach ($comments as $key => $comment):
                                    print($comment['value']);
                                endforeach;
                            ?>
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-xs-12">
                <?php
                print render($content['comments']); ?>
            </div>
        </div>
    </div>
    <div class="col-xs-4">
        <div class="row">
            <div class="col-xs-12">
                <div class="panel panel-default">
                    <div class="panel-heading">
                        <h3 class="panel-title">Resource Links</small></h3>
                    </div>
                    <div class="panel-body">
                        <div class="row">
                            <?php
                            foreach ($links as $key => $link):
                                $class = "resource-link resource-type-" . $field_resource_type[0]['value'] . ' ';
                                $class .= (strstr($val, 'google')) ? 'resource-google' : ''
                                ?>
                                <div class="col-xs-4 resource-link-con">
                                    <a class="<?php print $class;?>" href="<?php print $resource;?>" target="_blank">
                                        <span class="center-block"></span>

                                        <p class="text-center"><?php print $resource;?></p></a>
                                </div>
                                <?php
                            endforeach;
                            ?>
                        </div>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-heading">
                        <h3 class="panel-title">Skills</small></h3>
                    </div>
                    <div class="panel-body">
                        <div class="row">
                            <?php
                            foreach ($skills as $key => $skill):
                                $skill_node = $skill['node'];
                                $title = $skill_node->title;
                                $icon_uri = (isset($skill_node->field_icon['und'])) ? $skill_node->field_icon['und'][0]['uri'] : false;
                                $url = 'skills/' . $skill_node->nid;
                                if ($icon_uri) {
                                    $icon_url = file_create_url($icon_uri);
                                    $image_theme_array = array(
                                        "path" => $icon_url,
                                        "attributes" => array(
                                            "data-toggle" => "tooltip",
                                            "data-placement" => "top",
                                            "title" => $title,
                                            "width" => 25,
                                        ),
                                    );
                                    $skill_output = l(theme_image($image_theme_array), $url, array("html" => true));
                                } else {
                                    $skill_cat_nid = $skill_node->field_skill_category['und'][0]['nid'];
                                    $skill_output = l($title, $url, array('attributes' => array('class' => 'skill-link')));
                                }

                                ?>
                            <?php
                            endforeach;
                            ?>
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-xs-12">
                <div class="panel panel-default">
                    <div class="panel-heading">
                        <h3 class="panel-title">Instructor Comments</h3>
                    </div>
                    <div class="panel-body">
                        <div class="row">
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
