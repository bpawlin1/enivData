package com.environmentaldata.environmentaldata;
 
import java.util.List;
 
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
 
@Service
@Transactional
public class Users_service {
 
    @Autowired
    private UserRepository repo;
     
    public List<users> listAll() {
        return repo.findAll();
    }
     
    public void save(users users) {
        repo.save(users);
    }
     
    public users get(long user_id) {
        return repo.findById(user_id).get();
    }
     
    public void delete(long user_id) {
        repo.deleteById(user_id);
    }
}