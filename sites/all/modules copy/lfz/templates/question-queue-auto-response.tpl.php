<?php
$intro_sentence = "Thank you for submitting your question to the queue.";
$contact_sentence = $assigned_user . " will contact you shortly. ";
$post_message = "If you find the answer before a instructor is able to help you please resolve your question in the portal ". l("(Click Here)", "my-queue").".";

$random_index = 0;

$funny_messages = array(
    0=>array(
        "Grab a granola bar and coffee and tell Bill how good his hair looks today",
        "Challenge another student to a game of badmitton and the first student to break something get to buy coffee for everyone in class.",
        "When's the last time you went on social media and told everyone how amazing LearningFuze is? Well stop wasting time then!",
        "When Eric gets in he will help you quicker if he has coffee. Gifts/Bribes are the best way to get help ASAP. He is only human after all!",
    ),
    1=>array(
        "BTW, Dan likes to help students who can sing (don't ask questions) just start singing!",
        "If Scott could smoke while helping students he would never leave, step outside and check if Scott is helping someone already next to the no smoking sign.",
        "Tell Eric that you want to play FIFA and then when you have his attention make him feel guilty for not helping you sooner. FYI, he won't feel guilty and will probably go play FIFA without you.",
        "If you want to annoy Scott tell him that you no longer need to know CSS because you learned bootstrap.",
        "If your up for a challenge, try to convince Dan that magic numbers are acceptable only in a for loop. If he argues with you tell him Eric said he wouldn't understand.",
        "Its a race! If you find an answer to your question before Dan uses an analogy you get 500 LearningFuze dollars that can't be used at your local Starbucks!"
    ),
    2=>array(
        "If you can answer this question you will get helped faster. How much wood could a woodchuck wood if a woodchuck could chuck Dan?",
        "I bet Dan is still here, you should just go stare at him till he helps you!",
        "A 2 liter bottle of diet coke will get Scott to do your work for you.",
        "Next time you see Eric, plead your case for using alerts instead of a console log. Proceed with caution!",
        "Attempt to trip an instructor that is likely running for the door once they see this question in the queue."
    )
);

switch ($duty_status) {
    case -1:
        $contact_sentence = "Currently there is no one monitoring the queue. Questions submitted after business hours (8:30am - 8:30 M-F) will be answered the next business day.";
        break;
    case 0:
        $random_index = rand(0, count($funny_messages[0])-1);
        $contact_sentence = "Business hours have started and the instructors are probably making coffee or helping students who have waited at the top of the stairs to get help after a sleepless night. ";
        $contact_sentence .= $funny_messages[0][$random_index];
        break;
    case 1:
        $random_index = rand(0, count($funny_messages[1])-1);
        $contact_sentence .= $funny_messages[1][$random_index];
        break;
    case 2:
        $contact_sentence = $assigned_user." will be getting too you shortly.";
        $random_index = rand(0, count($funny_messages[2])-1);
        $contact_sentence .= $funny_messages[2][$random_index];
        break;
}

?>
<p>
    <?php print $intro_sentence; ?><br/>
    <?php print $contact_sentence; ?>
</p>
<p>While you're waiting, here are a couple links that may be helpful.</p>
<ul>
    <li>Search other student questions : <?php print l("Portal Question Answers", "recently-answered"); ?></li>
    <li>Track your questions status : <?php print l("Portal Help Queue", "my-queue"); ?></li>
    <li>While waiting do a google search : <?php print l("Click here for direct google search link", $google_search_link); ?></li>
</ul>
<p><?php print $post_message; ?></p>