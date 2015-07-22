<div class="skill-assessment-list list-group">
<?php
/**
 * Created by PhpStorm.
 * User: ericjohnson
 * Date: 7/6/15
 * Time: 5:10 PM
 */

if(count($data)>0){
    foreach($data as $item){
        print theme("skill_assessment_list_item", array("item"=>$item));
    }
}else{
    print t("No Skill Assessment results Available");
}
?>
</div>