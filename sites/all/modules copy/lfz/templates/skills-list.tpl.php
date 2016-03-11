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
                $icon_uri = (isset($skill_node->field_icon)) ? $skill_node->field_icon['und'][0]['uri'] : false;
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
                <div class="col-xs-4 skill-link-con">
                    <?php print $skill_output;?>
                </div>
            <?php
            endforeach;
            ?>
        </div>
    </div>
</div>