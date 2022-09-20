package com.environmentaldata.environmentaldata;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
@Transactional
public class co2_service {

	@Autowired
    private co2Data_DAO repo;


public List<co2_data> listAll() {
    return repo.findAll();
	}

public long count () {
	return repo.count();
}

public List<co2_data> findFirstByOrderByDatetimeDesc()
{
	return repo.findFirstByOrderByDatetimeDesc();
	
}

public List<co2_data> findlast24Hrs()
{
	return repo.findLast288ByOrderByIdAsc();
	
}

public List<Map<String, Object>> report() 
{
	List<Map<String, Object>> result = new ArrayList<Map<String, Object>>();
	
	for (co2_data product : repo.findAll()) 
	{
		Map<String, Object> item = new HashMap<String, Object>();
		
		item.put("device", product.getDevice());
		item.put("datatime", product.getDatetime());
		item.put("temp", product.getTemp());
		item.put("co2", product.getCo2());
		item.put("co2_rating", product.getCo2_rating());
		
		result.add(item);
	}
	return result;
}



}


