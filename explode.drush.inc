<?php

/**
 * Implements hook_drush_command() 
 */
function explode_drush_command(){
	$items = array();

  $items['explode-base-list-role-name'] = array(
    'description' => "List all roles and their name",
    'examples' => array(
      'drush expllr' => 'List of all roles',
      ),
    'aliases' => array('expllr'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
    );

	$items['explode-base-list'] = array(
	  'description' => "List all users and their role",
	  'arguments' => array(
		  'uid' => "User id user",
		  ),
	  'examples' => array(
	    'drush expll 3456' => 'All users and their roles, or the user and their role if the uid is added',
		  ),
	  'aliases' => array('expll'),
	  'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
	  );

  $items['explode-base-delete'] = array(
    'description' => "Delete Users Role",
    'arguments' => array(
      'uid' => "User id user whos role is being deleted",
      'role' => "Role that is being deleted"
      ),
    'examples' => array(
      'drush expld 3456 3' => 'Delete Role 3 from user 3456',
      ),
    'aliases' => array('expld'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
    );

  $items['explode-base-update'] = array(
    'description' => "Update a role of a user",
    'arguments' => array(
      'uid' => "User id user whos role is being updated",
      'role' => "Role that is being updated",
      ),
    'options' =>  array(
      'role-update' => 'Role which to update to', 
      ),
    'examples' => array(
      'drush explu 3456 3 --role-update=5' => 'Update role 3 to 5 on user 3456',
      ),
    'aliases' => array('explu'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
    );

  $items['explode-base-insert'] = array(
    'description' => "Not sure exacly what yet, something is exploding and shit",
    'arguments' => array(
      'uid' => "User id user whos role is being added",
      'role' => "Role that is being added"
      ),
    'examples' => array(
      'drush expla 3456 3' => 'Add role 3 to user 3456',
      ),
    'aliases' => array('expli'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
    );
	return $items;
}

/**
 *updates a role  
 */
function drush_explode_base_update($uid = "", $role = ""){
  $roleUpdate = drush_get_option('role-update');
  if ($uid === "" || $role === "" || $roleUpdate === ""){
    drush_print("uid or role id or role id to replace it with were provided");
    drush_user_abort(); 
  }else{
    $updateRole = db_query("UPDATE users_roles SET rid = :ridu WHERE uid = :uid AND rid = :rid", array(
      ':uid' => $uid, ':rid' => $role, ':ridu' => $roleUpdate));
  } 
}

/**
 *Creates a role the given user
 */
function drush_explode_base_insert($uid = "", $role = ""){
  if ($uid === "" || $role === ""){
    drush_print("uid or role id were not provided");
    drush_user_abort(); 
  }else{
      $inserUserRole = db_insert('users_roles')
        ->fields(array(
          'uid' => $uid,
          'rid' => $role,
      ))
        ->execute();
  }
}

/**
 * Deletes role if available
 */
function drush_explode_base_delete($uid = "", $role = ""){
  if ($uid === "" || $role === ""){
    drush_print("uid or role id were not provided");
    drush_user_abort(); 
  }else{
    if( drush_confirm("Are you sure you want to delete role ".$role."from".$uid."?")){
      $deleteUserRole = db_query("DELETE FROM users_roles WHERE rid = :rid AND uid = :uid", array(':uid' => $uid, ':rid' => $role));
    }
  }
}

/**
 * Lists users available 
 */
function drush_explode_base_list($uid = ""){
  
  if ($uid === ""){
    $query = db_query("SELECT users.name AS username, users.uid, role.name, users_roles.rid FROM users INNER JOIN users_roles ON users.uid = users_roles.uid INNER JOIN role ON role.rid = users_roles.rid");	
  }else{
    $userrole = db_query("SELECT users.name AS username, users.uid, role.name as role_name, users_roles.rid  FROM users_roles INNER JOIN users ON users.uid = :uid AND users_roles.uid = :uid INNER JOIN role ON role.rid = users_roles.rid", array(':uid' => $uid));
  }
  if ($uid === "" && drush_confirm("Print out all users since you did not specify a uid")){
    while ($result = $query->fetchObject()){
  	  drush_print_r($result);
  	}
  }elseif($uid != ""){
  	while ($result = $userrole->fetchObject()){
  	  drush_print_r($result);
  	}
  }else{
  	drush_user_abort();
  }
  drush_print("\n");
}

/**
 * Lists roles available
 */
function drush_explode_base_list_role_name(){

  $roles = db_query("SELECT rid, name FROM role");
  while ($result = $roles->fetchObject()){
    drush_print_r($result);
  }
}