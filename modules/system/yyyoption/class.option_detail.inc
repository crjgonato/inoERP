<?php

class option_detail {

 protected static $table_name = "option_detail";
 public static $primary_column = "option_detail_id";
 public static $field_array = [
		 "option_detail_id",
		 "option_header_id",
		 "option_line_id",
		 "option_detail_value",
		 "description",
		 "efid",
		 "status",
		 "rev_enabled",
		 "rev_number",
		 "effective_start_date",
		 "effective_end_date",
		 "created_by",
		 "creation_date",
		 "last_update_by",
		 "last_update_date"
 ];
 public static $initial_search_array = [
		 "option_header_id",
		 "option_line_id",
		 "option_detail_id",
		 "option_detail_value",
		 "description"
 ];
 public static $checkbox_array = [
 ];
 public static $column_array = [
		 "option_header_id",
		 "option_line_id",
		 "option_detail_id",
		 "option_detail_value",
		 "description"
 ];
 public static $base_encoded_array = [];
 public static $requiredField_array = [
		 "option_header_id",
		 "option_line_id",
		 "option_detail_value",
		 "description"
 ];
 public $option_detail_id;
 public $option_header_id;
 public $option_line_id;
 public $option_detail_value;
 public $description;
 public $efid;
 public $status;
 public $rev_enabled;
 public $rev_number;
 public $effective_start_date;
 public $effective_end_date;
 public $created_by;
 public $creation_date;
 public $last_update_by;
 public $last_update_date;
 public $time;
 public $msg;
 
  public function __set($name, $value) {
	$this->$name = $value;
 }

 
 public static function find_by_sql($sql = "") {
	global $db;
	$result_set = $db->query($sql);
	$object_array = array();
	while ($row = $db->fetch_array($result_set)) {
	 $object_array[] = self::instantiate($row);
	}//end of while Loop
	return $object_array;
 }

 public static function find_all_lines() {
	$sql = " SELECT * FROM " . self::$table_name;
	$result = self::find_by_sql($sql);
	return $result;
 }

 public static function count_all() {
	global $db;
	$sql = "SELECT COUNT(*) FROM " . self::$table_name;
	$result_set = $db->query($sql);
	$row = $db->fetch_array($result_set);
	return array_shift($row);
 }

 public static function find_by_id($option_line_id) {
	$sql = "SELECT * FROM " . self::$table_name . " where option_line_id={$option_line_id} LIMIT 1";
	$result_array = self::find_by_sql($sql);
	return !empty($result_array) ? array_shift($result_array) : false;
 }

 public static function find_by_option_header_id($option_header_id) {
	$sql = "SELECT * FROM " . self::$table_name . " where option_header_id = '{$option_header_id}'";
	$result_array = self::find_by_sql($sql);
	return $result_array;
 }
 
  public static function find_by_option_line_id($option_line_id) {
	$sql = "SELECT * FROM " . self::$table_name . " where option_line_id = '{$option_line_id}'";
	$result_array = self::find_by_sql($sql);
	return $result_array;
 }

 public static function find_by_option_line_id_value($option_line_id, $option_detail_value) {
	$sql = "SELECT * FROM " . self::$table_name .
					" where option_detail_value='{$option_detail_value}' AND option_line_id='{$option_line_id}' ";
	$result_array = self::find_by_sql($sql);
	return $result_array;
 }

 private static function instantiate($record) {
	$object = new self;
	foreach ($record as $attribute => $value) {
	 if ($object->has_attribute($attribute)) {
		$object->$attribute = $value;
	 }
	}
	return $object;
 }

 Private function has_attribute($attribute) {
	$object_vars = get_object_vars($this);
	return array_key_exists($attribute, $object_vars);
 }

 public static function install() {
	global $db;
	$sql = "
             CREATE TABLE IF NOT EXISTS `option_line` (
            `option_line_id` int(10) unsigned NOT NULL auto_increment,
            `option_id_l` int(12) NOT NULL,
            `option_line_code` varchar(50) NOT NULL,
            `value_l` varchar(50) NOT NULL,
            `description_l` varchar(255) default NULL,
            `efid_l` int(12) default NULL,
            `status_l` int(2) default '1',
            `rev_enabled_l` int(2) default '0',
            `rev_number_l` int(10) default NULL,
            `effective_start_date` varchar(50) default NULL,
            `effective_end_date` varchar(50) default NULL,
            `created_by_l` varchar(40) NOT NULL default '',
            `creation_date_l` varchar(50) default NULL,
            `last_update_by_l` varchar(40) NOT NULL default '',
            `last_update_date_l` varchar(50) default NULL,
            PRIMARY KEY  (`option_line_id`),
            UNIQUE KEY `option_code` (`option_line_code`),
            KEY `option_id` (`option_id_l`)
          ) ENGINE=MyISAM  DEFAULT CHARSET=latin1 ;
          ";
	$result = $db->query($sql);
	return !empty($result) ? $result : false;
 }

 public function save() {
	$check_option_line_value = $this->check_option_line_value($this->option_line_id);
	if (empty($this->option_detail_id)) {
	 	 if ($check_option_line_value == 0) {
		$this->create();
	 } else {
		$this->msg = "Duplicate option value";
	 }
	} else {
	 	 if ($check_option_line_value == 0) {
		$this->update($this->option_detail_id);
	 } else {
		$this->msg = "Duplicate option value";
	 }
	}
	return $this->msg;
 }

 private function check_option_line_value($option_line_id) {
	global $db;
	$sql = "SELECT * FROM " . self::$table_name . " where option_detail_value='{$this->option_detail_value}' ";
	$sql .= " AND option_line_id='{$option_line_id}'";
	$result_array = self::find_by_sql($sql);
	if (count($result_array) > 0) {
	 $this->msg = 1;
	} else {
	 $this->msg = 0;
	}
	return $this->msg;
 }

 public function create() {
	global $db;
	$field_arry_colums = array();
	$field_arry_values = array();
	foreach (self::$field_array as $key => $value) {
	 $field_arry_colums[] = sprintf("`%s`", $value);
	}
	$field_colums = implode(',', $field_arry_colums);

	foreach (self::$field_array as $key => $value) {
	 if (!empty($this->$value)) {
		$field_arry_values[] = sprintf("'%s'", $this->$value);
	 } else {
		$field_arry_values[] = 'NULL';
	 }
	}
	$field_values = implode(',', $field_arry_values);

	$sql = " INSERT INTO ";
	$sql.= self::$table_name;
	$sql.= ' ( ';
	$sql.= $field_colums;
	$sql.= ' ) ';
	$sql.= 'VALUES';
	$sql.= ' ( ';
	$sql.= $field_values;
	$sql.= ' ) ';


	if ($db->query($sql)) {
	 $this->option_detail_id = $db->insert_id();
	 $this->msg = 1;
	} else {
	 $this->msg = 0;
	}
	return $this->msg;
 }

//end of create view

 public function update($option_detail_id) {
	global $db;
	$field_arry_values = array();

	$time = time();
	$his->last_update_date = strftime("%Y-%m-%d %H:%M:%S", $time);
	$this->last_update_by = $_SESSION['user_name'];
	
	foreach (self::$field_array as $key => $value) {
	 if (isset($this->$value)) {
		$field_arry_values[] = sprintf("`%s` = '%s'", $value, $this->$value);
	 }
	}
	$field_values = implode(',', $field_arry_values);

	

	$sql = " UPDATE ";
	$sql.= self::$table_name;
	$sql .= " SET ";
	$sql.= $field_values;
//    $sql.= "`last_update_date` = '{$last_update_date}'," ;
//    $sql.= "`last_update_by` = '{$last_update_by }'" ;
	$sql.= " WHERE `option_detail_id` = '{$option_detail_id}' ";

//    echo "<br/>$sql is <br/>" . $sql;

	if ($db->query($sql)) {
	 $this->msg = 2;
	} else {
	 $this->msg = 0;
	}
	return $this->msg;
 }

 public static function delete($option_detail_id) {
	global $db;
	$option_detail_id = $db->escape_value($option_detail_id);
	$sql = "DELETE FROM `option_line` WHERE `option_detail_id` = '{$option_detail_id}' ";

	if ($db->query($sql)) {
	 $msg = 1;
	} else {
	 $msg = 0;
	}
	return $msg;
 }

}

//end of path class
?>