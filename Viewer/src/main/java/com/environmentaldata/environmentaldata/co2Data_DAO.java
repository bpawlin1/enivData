package com.environmentaldata.environmentaldata;


import java.util.List;
import org.springframework.data.jpa.repository.JpaRepository;


public interface co2Data_DAO extends JpaRepository<co2_data, Long>{
	
	public  List<co2_data> findFirstByOrderByDatetimeDesc();
	
	
	public  List<co2_data> findLast288ByOrderByIdAsc();
	


}



